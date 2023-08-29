#include "lexer.h"

using namespace lexer;

/*
 *  Location
 */
std::string Location::print() {
    return std::to_string(line) + "," + std::to_string(column);
}







/*
 *  Token
 */
void Token::process() {
    if (expresion == "fn")          type = fn;
    else if (expresion == "return") type = return_;
    else if (expresion == "mut")    type = mut;
    else if (expresion == "static") type = static_;
    else if (expresion == "raw")    type = raw;
    else if (expresion == "\n")     { type = eol_; expresion = "eol"; }
    else                            type = name;
}






/*
 *  Lexer
 */
Lexer::Lexer(std::ifstream &file) :
    file(file)
{}

Token Lexer::next() {
    Token token;
    char ch;

    while (file.peek() == ' ') {
        current_location.column++;
        file.ignore();
    }

    while (1) {
        ch = file.get();

        if (file.eof()) {
            return Token { eof_, "EOF", current_location };
        }

        current_location.column++;
        if (ch == ' ') break;
        token.expresion += ch;
        if (file.peek() == '\n' || ch == '\n') {
            token.location = current_location;
            token.location.column -= token.expresion.size() - 1;
            token.process();
            if (ch == '\n') {
                current_location.line++;
                current_location.column = 0;
            }
            return token;
        };
    }

    token.process();
    token.location = current_location;
    token.location.column -= token.expresion.size();
    return token;
}

void Lexer::to_file(std::string file_name) {
    std::ofstream output(file_name);
    Token token;

    while (token.type != eof_) {
        token = next();
        output << token.expresion << "     " << token.location.print() << std::endl;
    }
}
