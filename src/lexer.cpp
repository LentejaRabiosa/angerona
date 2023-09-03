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
    else if (expresion == "if")     type = if_;
    else if (expresion == "else")   type = else_;
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

        if (ch == ' ') {
            token.process();
            token.location = current_location;
            token.location.column -= token.expresion.size();
            return token;
        }

        token.expresion += ch;

        auto next_ch = file.peek();
        if (next_ch == ';' || openers.find(next_ch) != std::string::npos || closers.find(next_ch) != std::string::npos) break;
        if (ch == ';') {
            token.type = delimiter;
            break;
        }

        if (openers.find(ch) != std::string::npos) {
            token.type = opener;
            break;
        }

        if (closers.find(ch) != std::string::npos) {
            token.type = closer;
            break;
        }

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

    if (token.type == none) token.process();
    token.location = current_location;
    token.location.column -= token.expresion.size() - 1;
    return token;
}

void Lexer::to_file(std::string file_name) {
    std::ofstream output(file_name);
    Token token;

    while (token.type != eof_) {
        token = next();

        std::string spaces;
        for (int i = 10 - token.expresion.size(); i > 0; i--) spaces += ' ';

        output << token.expresion << spaces << token.location.print() << "  " << token.type << std::endl;
    }
}
