#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <any>
#include <variant>
#include <memory>
#include <exception>
#include <stdexcept>

#include "errors.h"
#include "lexer.h"
#include "files.h"
#include "toml.h"

namespace parser {

    class Variable {
        public:
        std::string name;
        std::any type;
        bool mut = false;   // mutable
        bool sta = false;   // static
        std::any value;

        void to_file(toml::Writer &writer);
    };

    class Node {
        std::shared_ptr<Node> parent;
        std::vector<std::shared_ptr<Node>> childs;

        std::any value;
        std::map<std::string, std::shared_ptr<Node>> types;
    };

    class Parser {
        typedef std::map<std::string, std::shared_ptr<std::ifstream>> files_map;

        files_map files;
        std::vector<std::string> extensions {"aro"};

        std::vector<lexer::Token> queue;
        Node init_node;
        toml::Writer writer {"test.parser.toml"};

        // TODO: try_parse_type();
        Variable try_parse_variable_declaration();

        public:
        Parser(std::vector<std::string> file_names);
    };

} // namespace parser

#endif
