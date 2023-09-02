#ifndef STYLE_H
#define STYLE_H

#include <string>

namespace color {

    enum Color {
        Black = 30,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
    };

}

namespace decorator {

    enum Decorator {
        None = 0,
        Bold = 1,
        Underline = 4,
        Inverse = 7,
    };

}

namespace style {

    typedef struct {
        color::Color color;
        decorator::Decorator decorator;
    } Style;

    std::string set(Style style, std::string text);
    std::string set(color::Color color, decorator::Decorator decorator, std::string text);

    std::string error();
    std::string error(std::string text);
}

#endif
