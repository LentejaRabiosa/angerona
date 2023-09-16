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
        CompoundType type;
        std::any value;

        void to_file(toml::Writer &writer);
    };

    class Node {
        public:
        std::shared_ptr<Node> parent;
        std::vector<std::shared_ptr<Node>> childs;
        std::any value;
    };












    enum MayParse {
        None,
        MayParseType,
        MayParseVariable,
        MayParseStruct,
        MayParseEnum,
        MayParseFunction,
    };

    typedef struct ParserResult {
        std::any ok;
        MayParse todo = None;
    } ParserResult;

    typedef std::vector<lexer::Token> Queue;
    typedef std::vector<lexer::Token>::iterator Queue_i;
    const std::set<std::string> default_types = {
        "fn",
        "char",
        "i32",
    };

    class Parser {
        toml::Writer &writer;
        Queue &queue;
        Node root;

        ParserResult try_parse(Queue_i &start);
        ParserResult try_parse_type(Queue_i &start);
        ParserResult try_parse_variable(Queue_i &start, CompoundType type);
        ParserResult try_parse_struct(Queue_i &start);

        public:
        Parser(Queue &queue, toml::Writer &writer);
        void parse();
    };

    class FilesParser {
        typedef std::map<std::string, std::shared_ptr<std::ifstream>> files_map;

        files_map files;
        std::vector<std::string> extensions {"aro"};
        toml::Writer writer { "test.parser.toml" };

        public:
        FilesParser(std::vector<std::string> file_names);
    };

} // namespace parser

#endif
