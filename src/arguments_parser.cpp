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
    
    auto distance = std::distance(start, end);
    if (range.max > distance < range.min) throw error::arguments_parser::TooFewArguments();

    while (range.min < range.max) {
        values.push_back(*start);
        start++;
        range.min++;
    }

    return start;
}









/*
 *  Group
 */
Group& Group::set_title(std::string t) {
    title = t;
    return *this;
}

Group& Group::set_description(std::string d) {
    description = d;
    return *this;
}

Group& Group::set_epilog(std::string e) {
    epilog = e;
    return *this;
}

std::string Group::print() const {
    std::stringstream ss;

    if (!title.empty()) ss << title << std::endl;
    if (!description.empty()) ss << description << std::endl;

    for (const auto &label : labels) {
        ss << "  ";
        for (const auto &name : label.names) ss << name << "  ";
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

void Parser::set_description(std::string d) {
    description = d;
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
        label.type = Optional;
    } else {
        for (const auto &name : names) optionals_map.emplace(name, argument);
        label.type = Positional;
    }

    group_i->second.labels.push_back(label);

    return *argument;
}

void Parser::add_subparser(Parser& parser) {
    auto subparser = subparsers.emplace(subparsers.end(), parser);
    used_commands.emplace(parser.names[0], false);
    commands.emplace(parser.names[0], subparser);

    Label label {parser.names, parser.description, Command};
    commands_group.labels.push_back(label);
}

void Parser::parse(int argc, const char **const argv) {
    parse({argv, argv + argc});
}

void Parser::parse(std::vector<std::string> raw_names) {
    auto positionals_i = positionals.begin();
    for (auto name = std::next(raw_names.begin()); name != raw_names.end(); name++) {
        const auto &arg = *name;

        if (const auto &optional_i = optionals_map.find(arg); optional_i != optionals_map.end()) {
            // OPTIONAL
            optional_i->second->is_used = true;

        } else if (const auto &subparser_i = commands.find(arg); subparser_i != commands.end()) {
            // COMMAND
            subparser_i->second->get().parse(std::vector(name, raw_names.end()));
            used_commands[arg] = true;
            break;

        } else if (positionals_i != positionals.end()) {
            // POSITIONAL
            positionals_i->is_used = true;
            positionals_i->value = std::string(arg);
            positionals_i++;

        } else throw error::arguments_parser::InvalidArgument(arg);
    }

    // validate
    if (positionals_i != positionals.end()) throw error::arguments_parser::MissingArguments(positionals_i->names.front());

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

bool Parser::is_command_used(std::string command) const {
    return used_commands.find(command) != used_commands.end();
}

std::string Parser::used_command() const {
    for (const auto &command_i : used_commands) if (command_i.second) return command_i.first;
    return "";
}

Argument Parser::operator[](std::string name) {
    if (const auto &optional_i = optionals_map.find(name); optional_i != optionals_map.end()) return *optional_i->second;
    if (const auto &positional_i = positionals_map.find(name); positional_i != positionals_map.end()) return *positional_i->second;
    throw std::runtime_error("Unknown argument");
}

std::vector<std::string> Parser::get_values(std::string name) {
    auto argument = *this[name];

}
