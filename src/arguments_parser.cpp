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
Argument::Argument()
{}

Argument::Argument(std::vector<std::string> names) :
    names(names)
{}









/*
 *  Group
 */
Group::Group(std::string title) :
    title(title)
{}

std::string Group::print() {
    std::stringstream ss;

    ss << title << ":" << std::endl;;
    for (const auto &argument : arguments) {
        ss << "  ";
        for (const auto &name : argument->names) {
            ss << style::set(color::White, decorator::Underline, name) << "  ";
        }
        ss << argument->help << std::endl;
    }

    return ss.str();
}







/*
 *  Parser
 */
Parser::Parser(std::string name) :
    name(name)
{}

Argument& Parser::add_argument(std::vector<std::string> names, std::string group) {
    auto argument = optionals.emplace(optionals.end(), Argument(names));

    if (!is_optional(names, prefix_chars)) positionals.splice(positionals.end(), optionals, argument);

    if (const auto &group_i = groups_map.find(group); group_i != groups_map.end()) group_i->second->arguments.push_back(argument);
    else throw std::runtime_error("Unknown group name");

    for (const auto &name : names) arguments.emplace(name, argument);
    return *argument;
}

Group& Parser::add_group(std::string group, std::string title) {
    auto group_i = groups.emplace(groups.end(), Group(title));
    groups_map.emplace(group, group_i);
    return *group_i;
}

void Parser::parse(int argc, const char **const argv) {
    parse({argv, argv + argc});
}

void Parser::parse(std::vector<std::string> raw_names) {
    auto positionals_i = positionals.begin();
    for (auto name = std::next(raw_names.begin()); name != raw_names.end(); name++) {
        const auto &arg = *name;

    }
}

void Parser::help(std::vector<std::string> group_names) {
    std::stringstream help_ss;

    for (const auto &group_name : group_names) {
        if (const auto &group_i = groups_map.find(group_name); group_i == groups_map.end()) throw std::runtime_error("Unknonw gruop name");
        else help_ss << group_i->second->print() << std::endl;
    }

    std::cout << help_ss.str() << std::endl;
}
