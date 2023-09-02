#include "style.h"

using namespace color;
using namespace decorator;

std::string style::set(Style style, std::string text) {
    return std::string("\033[")
        + std::to_string(style.decorator)
        + std::string(";")
        + std::to_string(style.color)
        + std::string("m")
        + text
        + std::string("\033[0m");
}

std::string style::set(Color color, Decorator decorator, std::string text) {
    return std::string("\033[")
        + std::to_string(decorator)
        + std::string(";")
        + std::to_string(color)
        + std::string("m")
        + text
        + std::string("\033[0m");
}

std::string style::error() {
    return style::set(color::Red, decorator::Bold, "error:");
}

std::string style::error(std::string text) {
    return style::set(color::Red, decorator::Bold, "error:") + text;
}
