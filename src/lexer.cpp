#include "lexer.h"

using namespace lexer;

/*
 * Lexer
 */
std::string Lexer::get_extension(std::string file_name) {
    return file_name.substr(file_name.find_last_of(".") + 1);
}

Lexer::Lexer(std::vector<std::string> file_names) {
    for (const auto &name : file_names) {
        auto extension = get_extension(name);
        if (std::find(extensions.begin(), extensions.end(), extension) == extensions.end()) throw error::lexer::UnknownFileExtension(name);

        if (files.find(name) == files.end()) throw error::lexer::DuplicatedFileName(name);
        std::ifstream file;
        file.open(name);
        files.emplace(name, file);
    }

    auto main_i = files.find("main.aro");
    if (main_i == files.end()) throw std::runtime_error("main.aro not found");
}

Item Lexer::consume(std::string name) {
    auto file_i = files.find(name);
    if (file_i == files.end()) throw std::runtime_error("File not found");
    auto &file = file_i->second;

    std::string expr;
    char ch;
    while((ch = file.peek()) != 0 || ch != ' ') {
        expr += ch;
    }

    Item item {expr};
    if (expr == "fn")          item.token = fn;
    else if (expr == "i32")    item.token = i32;
    else if (expr == "return") item.token = return_;

    return item;
}
