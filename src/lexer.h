#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <memory>

#include "errors.h"

namespace lexer {

    typedef struct Location {
        unsigned int line;
        unsigned int column;

        std::string print();
    } Location;

    enum TokenType {
        none,
        eof_,
        eol_,
        indent,
        name,
        symbol,
        operator_,
        delimiter,   //  ;
        opener,      //  ([{<
        closer,      //  )]}>
        fn,
        struct_,
        return_,
        if_,
        else_,
        mut,
        static_,
        raw,
    };

    typedef struct Token {
        TokenType type = none;
        std::string expresion;
        Location location;

        void process();
    } Token;

    class Lexer {
        std::ifstream &file;
        Location current_location {1, 0};
        std::string openers {"([{<"};
        std::string closers {")]}>"};

        public:

        Lexer(std::ifstream &file);
        Token next();
        void to_file(std::string file_name);
    };

} // namespace lexer

#endif
