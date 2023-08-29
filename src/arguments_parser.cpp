#include "arguments_parser.h"

using namespace arguments_parser;


/*
 *  Checks if a given name is an optional valid name
 */
static bool  is_optional(std::string name, std::string prefix_chars) {
    if (name.empty()) return true;
    else return prefix_chars.find(name[0]) != std::string::npos;
}

/*
 *  Checks if a given vector of names are valid optionals (all of them)
 */
static bool is_optional(std::vector<std::string> names, std::string prefix_chars) {
    for (const auto &name : names) if (!is_optional(name, prefix_chars)) return false;
    return true;
}









/*
 *  Argument
 */
Argument::Argument(std::vector<std::string> names) :
    names(names)
{}

Argument& Argument::set_help(std::string h) {
    help = h;
    return *this;
}

Argument& Argument::remaining() {
    can_remain = true;
    range = {1, SIZE_MAX};
    return *this;
}

Argument::Argument_i Argument::consume(Argument_i start, Argument_i end) {
    is_used = true;
    
    auto min = range.min;
    auto max = range.max;
    auto distance = std::distance(start, end);
    if (distance < min) throw error::arguments_parser::TooFewArguments();

    while (values.size() < min && start != end) {
        values.push_back(*start);
        start++;
    }

    while (min < max && start != end) {
        values.push_back(*start);
        min++;
        start++;
    }

    return start;
}









/*
 *  Group
 */
std::string Group::print() const {
    std::stringstream ss;

    if (!title.empty()) ss << style::set(color::White, decorator::Bold, title) << std::endl;
    if (!description.empty()) ss << description << std::endl;

    for (const auto &label : labels) {
        ss << "  ";
        if (label.type == Positional) for (const auto &name : label.names) ss << "<" << name << ">  ";
        else if (label.type == Optional) for (const auto &name : label.names) ss << style::set(color::White, decorator::Underline, name) << "  ";
        else for (const auto &name : label.names) ss << name << "  ";
        ss << label.help << std::endl;;
    }

    if (!epilog.empty()) ss << epilog << std::endl;

    return ss.str();
}







/*
 *  Parser
 */
Parser::Parser(std::string name) {
    names.push_back(name);
}

void Parser::set_version(std::string v) {
    names.push_back(v);
}

Group& Parser::add_group(std::string group) {
    auto group_i = groups.emplace(group, Group());
    return group_i.first->second;
}

Argument& Parser::add_argument(std::vector<std::string> names, std::string group) {
    const auto &group_i = groups.find(group);
    if (group_i == groups.end()) throw std::runtime_error("Unknown group name");

    auto argument = optionals.emplace(optionals.end(), Argument(names));

    Label label {argument->names, argument->help};

    if (!is_optional(names, prefix_chars)) {
        positionals.splice(positionals.end(), optionals, argument);
        for (const auto &name : names) positionals_map.emplace(name, argument);
        label.type = Positional;
    } else {
        for (const auto &name : names) optionals_map.emplace(name, argument);
        label.type = Optional;
    }

    group_i->second.labels.push_back(label);

    return *argument;
}

void Parser::add_subparser(Parser& parser) {
    auto subparser = subparsers.emplace(subparsers.end(), parser);
    commands.emplace(parser.names[0], subparser);

    Label label {parser.names, parser.description, Command};
    commands_group.labels.push_back(label);
}

void Parser::parse(int argc, const char **const argv) {
    parse({argv, argv + argc});
}

void Parser::parse(std::vector<std::string> raw_names) {
    auto positionals_i = positionals.begin();
    for (auto name = std::next(raw_names.begin()); name != raw_names.end();) {
        const auto &arg = *name;

        if (arg == "--help" || arg == "-h") {
            help();
            throw error::arguments_parser::HelpAbort();
        }

        if (const auto &optional_i = optionals_map.find(arg); optional_i != optionals_map.end()) {
            // OPTIONAL
            name = optional_i->second->consume(++name, raw_names.end());

        } else if (const auto &subparser_i = commands.find(arg); subparser_i != commands.end()) {
            // COMMAND
            subparser_i->second->get().parse(std::vector(name, raw_names.end()));
            break;

        } else if (positionals_i != positionals.end()) {
            // POSITIONAL
            name = positionals_i->consume(name, raw_names.end());
            positionals_i++;

        } else throw error::arguments_parser::InvalidArgument(arg);
    }

    // validate
    if (positionals_i != positionals.end()) throw error::arguments_parser::MissingArguments();

    is_parsed = true;
}

void Parser::help() {
    std::stringstream ss;

    ss << std::endl;

    if (!subparsers.empty()) ss << commands_group.print();

    for (const auto &group_i : groups) {
        ss << group_i.second.print();
    }

    std::cout << ss.str();
}

void Parser::help(std::vector<std::string> group_names) {
    std::stringstream ss;

    ss << std::endl;

    if (!subparsers.empty()) ss << commands_group.print();

    for (const auto &group_name : group_names) {
        const auto &group_i = groups.find(group_name);
        if (group_i == groups.end()) throw std::runtime_error("Unknown group name");
        ss << group_i->second.print();
    }

    std::cout << ss.str();
}

Argument& Parser::get_argument(std::string name) {
    if (const auto &optional_i = optionals_map.find(name); optional_i != optionals_map.end()) return *optional_i->second;
    if (const auto &positional_i = positionals_map.find(name); positional_i != positionals_map.end()) return *positional_i->second;
    throw std::runtime_error("Unknown argument");
}

std::vector<std::string> Parser::get_values(std::string name) {
    return get_argument(name).values;
}

bool Parser::is_used(std::string name) {
    return get_argument(name).is_used;
}
