#include "parser.h"

using namespace parser;



/*
 *	Compound Type
 */
void CompoundType::to_file(toml::Writer &writer) {
    // writer.table();
    // writer.new_line();
}








/*
 *  Variable
 */
void Variable::to_file(toml::Writer &writer) {
    // writer.table("variable." + name);
    // writer.pair("static", sta);
    // writer.pair("mut", mut);
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

        while (token.type != lexer::eof_ && token.type != lexer::delimiter
                && token.type != lexer::opener && token.type != lexer::closer) {
            token = main_lexer.next();
            queue.push_back(token);
        }

        if (token.type == lexer::eof_) break;

        // for (const auto &x : queue) std::cout << x.expresion << std::endl;
        // std::cout << std::endl;

        auto type = try_parse_type(queue.begin());
        auto next = try_parse_after_type(queue.begin());
        if (next.type() == typeid(Variable)) {
            auto var = std::any_cast<Variable>(next);
            var.type = type;

            std::shared_ptr<Node> node(new Node);
            node->parent = init_node;
            node->value = var;
            init_node->childs.push_back(node);
        }

        queue.clear();
    }
}

CompoundType Parser::try_parse_type(queue_i start) {
    CompoundType ct;
    Type type_info;
    std::any type;

    for (;start != queue.end(); start++) {
        if (start->type == lexer::static_) {
            if (type_info.sta) throw error::parser::DuplicatedKeyword(start->expresion);
            if (type_info.mut) throw error::parser::BadKeywordsOrder("mut", start->expresion);
            type_info.sta = true;

        } else if (start->type == lexer::mut) {
            if (type_info.mut) throw error::parser::DuplicatedKeyword(start->expresion);
            type_info.mut = true;

        } else if (start->type == lexer::name) {
            if (default_types.find(start->expresion) == default_types.end()) throw std::runtime_error("bad type");
            if (start->expresion == "fn") Fn();
            else if (start->expresion == "char") Char();
            else if (start->expresion == "i32") I32();

        } else if (start->type != lexer::eol_ && start->type != lexer::delimiter) {
            throw std::runtime_error("invalid expresion");
        }
    }

    ct.info = type_info;
    ct.type = type;
    return ct;
}



std::any Parser::try_parse_after_type(queue_i start) {
    for (;start != queue.end(); start++) {
        if (start->type == lexer::name) {
            // TODO: check if valid name
            Variable var;
            var.name = start->expresion;
            return var;

        } else if (start->type == lexer::struct_ && (start++)->type == lexer::name) {
            return try_parse_struct(start);
        }
    }

    return false;
}



Struct Parser::try_parse_struct(queue_i start) {
    Struct s;

    s.name = start->expresion;
    for (;start != queue.end(); start++) {
        auto type = try_parse_type(start);
        
    }

    return s;
}





Variable Parser::try_parse_variable_declaration(queue_i start) {
    Variable var;
    bool nrp = false;

    for (;start != queue.end(); start++) {
        // if (start->type == lexer::static_) {
        //     if (var.sta) throw error::parser::DuplicatedKeyword(start->expresion);
        //     if (var.mut) throw error::parser::BadKeywordsOrder("mut", start->expresion);
        //     if (nrp)     throw std::runtime_error("static is after name declaration");
        //     var.sta = true;

        // } else if (start->type == lexer::mut) {
        //     if (var.mut) throw error::parser::DuplicatedKeyword(start->expresion);
        //     if (nrp)     throw std::runtime_error("mut is after name declaration");
        //     var.mut = true;

        // } else if (start->type == lexer::name && !var.type.has_value()) {
        //     // TODO: check and get the type
        //     nrp = true;
        //     var.type = std::string("some example type XD");

        // } else if (start->type == lexer::name) { // TODO: check if valid name
        //     var.name = start->expresion;

        // } else if (start->type == lexer::operator_) {
        //     // TODO: process next token as the value
        //     nrp = true;
        //     var.value = (start++)->expresion;

        // } else if (start->type != lexer::eol_ && start->type != lexer::delimiter) {
        //     throw std::runtime_error("invalid expresion");
        // }
    }

    var.to_file(writer);

    return var;
}
