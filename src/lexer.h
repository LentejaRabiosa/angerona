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

#include "errors.h"

namespace lexer {

    enum Token {
        eof_,
        eol_,
        indent,
        symbol,
        operator_,
        fn,
        i32,
        return_,
        if_,
        else_,
    };

    typedef struct {
        std::string expresion;
        Token token;
    } Item;

    class Lexer {
        typedef std::map<std::string, std::ifstream> files_map;
        files_map files;
        std::vector<std::string> extensions {"aro"};
        std::string get_extension(std::string file_name);

        public:
        Lexer(std::vector<std::string> file_names);
        Item consume(std::string name);
    };

} // namespace lexer

#endif
