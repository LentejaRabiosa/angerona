#include "toml.h"

using namespace toml;

Writer::Writer(std::string file_name) :
    file(file_name)
{}

void Writer::table(std::string value) {
    file << "[" << value << "]" << std::endl;
}

void Writer::pair(std::string key, std::string value) {
    file << key << " = " << value << std::endl;;
}

void Writer::pair(std::string key, bool value) {
    file << key << " = ";

    if (value) file << "true";
    else file << "false";

    file << std::endl;
}

void Writer::new_line() {
    file << std::endl;
}
