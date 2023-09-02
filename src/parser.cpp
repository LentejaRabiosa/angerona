#include "parser.h"

using namespace parser;

/*
 *  Variable
 */
void Variable::to_file(toml::Writer &writer) {
    writer.table("variable." + name);
    writer.pair("static", sta);
    writer.pair("mut", mut);
    writer.new_line();
}









/*
 *  Parser
 */
Parser::Parser(std::vector<std::string> file_names) {
    for (const auto &name : file_names) {
        auto extension = files::extension(name);
        if (std::find(extensions.begin(), extensions.end(), extension) == extensions.end()) throw error::files::UnknownFileExtension(name);

        if (files.find(name) != files.end()) throw error::files::DuplicatedFileName(name);
        auto file = new std::ifstream(name);
        if (!file->is_open()) throw error::files::CouldNotOpenFile(name);
        files.emplace(name, file);
    }

    auto main_lexer = lexer::Lexer(*files.begin()->second);

    while (1) {
        lexer::Token token;

        while (token.type != lexer::eof_ && token.type != lexer::delimiter) {
            token = main_lexer.next();
            queue.push_back(token);
        }

        if (token.type == lexer::eof_) break;

        auto var = try_parse_variable_declaration();

        queue.clear();
    }
}

Variable Parser::try_parse_variable_declaration() {
    Variable var;
    bool nrp = false;

    for (auto start = queue.begin(); start != queue.end(); start++) {
        if (start->type == lexer::static_) {
            if (var.sta) throw error::parser::DuplicatedKeyword(start->expresion);
            if (var.mut) throw error::parser::BadKeywordsOrder("mut", start->expresion);
            if (nrp)     throw std::runtime_error("static is after name declaration");
            var.sta = true;

        } else if (start->type == lexer::mut) {
            if (var.mut) throw error::parser::DuplicatedKeyword(start->expresion);
            if (nrp)     throw std::runtime_error("mut is after name declaration");
            var.mut = true;

        } else if (start->type == lexer::name && !var.type.has_value()) {
            // TODO: check and get the type
            nrp = true;
            var.type = std::string("some example type XD");

        } else if (start->type == lexer::name) { // TODO: check if valid name
            var.name = start->expresion;

        } else if (start->type == lexer::operator_) {
            // TODO: process next token as the value
            nrp = true;
            var.value = (start++)->expresion;

        } else if (start->type != lexer::eol_ && start->type != lexer::delimiter) {
            throw std::runtime_error("invalid expresion");
        }
    }

    var.to_file(writer);

    return var;
}
