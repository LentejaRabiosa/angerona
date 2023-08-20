#ifndef ARGUMENTS_PARSER_H
#define ARGUMENTS_PARSER_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <any>
#include <functional>

#include "style.h"
#include "errors.h"

namespace arguments_parser {

    class Range {
        public:
        std::size_t min = 1;
        std::size_t max = 1;
    };

    class Argument {
        friend class Parser;

        std::vector<std::string> names;
        std::string help;
        std::string default_value;
        std::vector<std::string> values;
        bool is_optional = true;
        bool is_used = false;
        bool can_remain = false;
        Range range;

        public:
        typedef std::vector<std::string>::iterator Argument_i;
        Argument(std::vector<std::string> names);
        Argument& set_help(std::string h);
        Argument& remaining();
        Argument_i consume(Argument_i start, Argument_i end);
    };

    typedef std::list<Argument>::iterator Argument_i;

    enum ArgumentType {
        Positional,
        Optional,
        Command,
    };

    typedef struct {
        std::vector<std::string> &names;
        std::string &help;
        ArgumentType type;
    } Label;

    class Group {
        std::string title;
        std::string description;
        std::string epilog;

        public:
        std::vector<Label> labels;

        Group& set_title(std::string t);
        Group& set_description(std::string d);
        Group& set_epilog(std::string e);
        std::string print() const;
    };

    /*
     *  TODO: specific classes for Optionals and Positionals ??
     */
    class Parser {
        std::vector<std::string> names;
        std::string custom_usage;
        std::string description;
        std::string prefix_chars = "-";
        bool is_parsed = false;

        // arguments
        std::list<Argument> positionals;
        std::list<Argument> optionals;
        std::map<std::string, Argument_i> positionals_map;
        std::map<std::string, Argument_i> optionals_map;

        // subparsers
        typedef std::list<std::reference_wrapper<Parser>>::iterator Parser_i;
        std::list<std::reference_wrapper<Parser>> subparsers;
        std::map<std::string, Parser_i> commands;
        std::map<std::string, bool> used_commands;

        // groups
        std::map<std::string, Group> groups;

        Argument operator[](std::string name);

        public:
        Group commands_group;

        Parser(std::string name);
        void set_version(std::string v);
        void set_description(std::string d);
        Group& add_group(std::string group);
        Argument& add_argument(std::vector<std::string> names, std::string group);
        void add_subparser(Parser& parser);


        void parse(int argc, const char **const argv);
        void parse(std::vector<std::string> raw_arguments);


        void help();
        void help(std::vector<std::string> group_names);

        bool is_command_used(std::string command) const;
        std::string used_command() const;
        std::vector<std::string> get_values(std::string name);
    };

} // namespace arguments_parser

#endif
