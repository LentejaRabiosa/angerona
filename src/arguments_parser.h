#ifndef ARGUMENTS_PARSER_H
#define ARGUMENTS_PARSER_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <stdexcept>

#include "style.h"
#include "errors.h"

namespace arguments_parser {

    class Range {
        std::size_t min = 1;
        std::size_t max = 1;
    };

    enum RangeType {
        unique,
        optional,
        any,
        at_least_one,
    };

    /*
     *  TODO: special arguments with specific arrays and the maps are just like
     *  (key -> value)
     */
    class Argument {
        friend class Parser;
        friend class Group;

        std::vector<std::string> names;
        std::string help;
        std::vector<std::string> values;
        bool is_optional = false;
        bool is_used = false;
        Range range;

        public:
        Argument();
        Argument(std::vector<std::string> names);
    };

    typedef std::list<Argument>::iterator Argument_i;

    class Group {
        friend class Parser;

        std::string title;
        std::vector<Argument_i> arguments;

        public:
        Group(std::string title);
        std::string print();
    };

    class Parser {
        std::string name;
        std::string version;
        std::string custom_usage;
        std::string description;
        std::string prefix_chars = "-";
        bool is_parsed = false;

        // arguments
        std::list<Argument> positionals;
        std::list<Argument> optionals;
        std::map<std::string, Argument_i> arguments;

        // subparsers
        typedef std::list<Parser>::iterator Parser_i;
        std::list<Parser> subparsers;
        std::map<std::string, Parser_i> commands;

        // groups
        typedef std::list<Group>::iterator Group_i;
        std::list<Group> groups;
        std::map<std::string, Group_i> groups_map;

        public:
        Parser(std::string name);
        Argument& add_argument(std::vector<std::string> names, std::string group);
        Group& add_group(std::string group, std::string title);
        void parse(int argc, const char **const argv);
        void parse(std::vector<std::string> raw_arguments);
        std::string usage();
        void help(std::vector<std::string> group_names);
    };

} // namespace arguments_parser

#endif
