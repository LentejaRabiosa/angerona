#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <array>
#include <list>
#include <any>
#include <variant>
#include <memory>
#include <typeinfo>
#include <stdint.h>
#include <exception>
#include <stdexcept>

#include "errors.h"
#include "lexer.h"
#include "files.h"
#include "toml.h"

namespace parser {

    class Type {
        public:
        bool pub = false;
        bool sta = false;
        bool mut = false;
    };

    class Fn : public Type {
        void *data;
    };

    class Char : public Type {
        char data = 0;
    };

    class I32 : public Type {
        int32_t data = 0;
    };

    class CompoundType {
        public:
        Type info;
        std::any type;

        void to_file(toml::Writer &writer);
    };

    class CustomType {
        public:
        std::map<std::string, CompoundType> properties;
    };

    class Struct {
        public:
        std::string name;
        CustomType type;
    };











    
    class Variable {
        public:
        std::string name;
        std::any type;
        std::any value;

        void to_file(toml::Writer &writer);
    };

    class Node {
        public:
        std::shared_ptr<Node> parent;
        std::vector<std::shared_ptr<Node>> childs;
        std::any value;
    };

    class Parser {
        typedef std::map<std::string, std::shared_ptr<std::ifstream>> files_map;
        typedef std::vector<lexer::Token>::iterator queue_i;

        files_map files;
        std::vector<std::string> extensions {"aro"};

        std::vector<lexer::Token> queue;
        std::set<std::string> default_types = {
            "fn",
            "char",
            "i32",
        };
        std::shared_ptr<Node> init_node {new Node};
        toml::Writer writer {"test.parser.toml"};

        CompoundType try_parse_type(queue_i start);
        std::any try_parse_after_type(queue_i start);
        Struct try_parse_struct(queue_i start);

        Variable try_parse_variable_declaration(queue_i start);

        public:
        Parser(std::vector<std::string> file_names);
    };

} // namespace parser

#endif
