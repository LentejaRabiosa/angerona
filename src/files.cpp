#include "files.h"

std::string files::name(std::string path) {
    auto start = path.find_last_of("/") + 1;
    return path.substr(start, path.find_last_of(".") - start);
}

std::string files::full_name(std::string path) {
    return path.substr(path.find_last_of("/") + 1);
}

std::string files::extension(std::string path) {
    return path.substr(path.find_last_of(".") + 1);
}
