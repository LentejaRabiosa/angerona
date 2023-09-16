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
    writer.table("variable." + name);
    writer.pair("pub", type.info.pub);
    writer.pair("static", type.info.sta);
    writer.pair("mut", type.info.mut);
    writer.new_line();
}










/*
 *  Parser
 */
Parser::Parser(Queue &queue, toml::Writer &writer) : queue(queue), writer(writer)
{}

void Parser::parse() {
    auto start = queue.begin();

    while (start != queue.end()) {
        try_parse(start);
    }
}

ParserResult Parser::try_parse(Queue_i &start) {
    ParserResult result;

    if (start->type == lexer::name) {
        // Too many thing to try
        if (default_types.find(start->expresion) != default_types.end()) {  // TODO: refactor
            auto type = try_parse_type(start);
            if (type.ok.has_value()) {
                auto var = try_parse_variable(start, std::any_cast<CompoundType>(type.ok));
                result.ok = var;

            } else if (type.ok.has_value() && type.todo == MayParseStruct) {
                auto struct_ = try_parse_struct(start);
                result.ok = struct_;
            }
        }

    } else if (start->type == lexer::mut || start->type == lexer::static_ || start->type == lexer::pub) {
        auto type = try_parse_type(start);
        if (type.ok.has_value()) {
            auto var = try_parse_variable(start, std::any_cast<CompoundType>(type.ok));
            result.ok = var;

        } else if (type.ok.has_value() && type.todo == MayParseStruct) {
            auto struct_ = try_parse_struct(start);
            result.ok = struct_;
        };

    } else if (start->type == lexer::struct_) {


    } else if (start->type == lexer::enum_) {


    } else if (start->type == lexer::fn) {

    } else {
        start++;
    }

    return result;
}

ParserResult Parser::try_parse_type(Queue_i &start) {
    ParserResult result;
    CompoundType compound_type;
    Type type_info;
    std::any type;

    for (;start != queue.end(); start++) {
        if (start->type == lexer::pub) {
            if (type_info.pub) throw error::parser::DuplicatedKeyword(start->expresion);
            if (type_info.sta) throw error::parser::BadKeywordsOrder("sta", start->expresion);
            if (type_info.mut) throw error::parser::BadKeywordsOrder("mut", start->expresion);
            type_info.pub = true;

        } else if (start->type == lexer::static_) {
            if (type_info.sta) throw error::parser::DuplicatedKeyword(start->expresion);
            if (type_info.mut) throw error::parser::BadKeywordsOrder("mut", start->expresion);
            type_info.sta = true;

        } else if (start->type == lexer::mut) {
            if (type_info.mut) throw error::parser::DuplicatedKeyword(start->expresion);
            type_info.mut = true;

        } else if (start->type == lexer::struct_) {
            type = Struct();
            result.todo = MayParseStruct;
            start++;
            break;

        } else if (start->type == lexer::name) {
            if (start->expresion == "fn")          type = Fn();
            else if (start->expresion == "char")   type = Char();
            else if (start->expresion == "i32")    type = I32();
            else throw std::runtime_error("bad type");
            start++;
            break;

        } else if (start->type != lexer::eol_ && start->type != lexer::delimiter) {
            throw std::runtime_error("invalid expresion");
        }
    }

    compound_type.info = type_info;
    compound_type.type = type;
    result.ok = compound_type;
    return result;
}

ParserResult Parser::try_parse_variable(Queue_i &start, CompoundType type) {
    Variable var;

    for (;start->type != lexer::delimiter; start++) {
        if (start->type == lexer::name) {
            if (!var.name.empty()) throw error::parser::InvalidName(start->expresion);
            var.name = start->expresion;  // TODO: check if valid name
            var.type = type;
            
        } else if (start->type == lexer::operator_) {
            if (var.name.empty()) throw std::runtime_error("operator not after name");
            // TODO: parse value

        } else throw error::parser::InvalidName(start->expresion);
    }

    var.to_file(writer);

    return ParserResult { var };
}

ParserResult Parser::try_parse_struct(Queue_i &start) {
    std::cout << start->expresion << std::endl;
    throw std::runtime_error("caca");

    for (;start != queue.end(); start++) {

    }
}

 















/*
 *  Files Parser
 */
FilesParser::FilesParser(std::vector<std::string> file_names) {
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
        Queue queue;

        while (token.type != lexer::eof_ && token.type != lexer::delimiter
                && token.type != lexer::opener && token.type != lexer::closer) {
            token = main_lexer.next();
            queue.push_back(token);
        }

        if (token.type == lexer::eof_) break;

        // for (const auto &x : queue) std::cout << x.expresion << std::endl;
        // std::cout << std::endl;

        Parser parser(queue, writer);
        parser.parse();

       queue.clear();
    }
}
