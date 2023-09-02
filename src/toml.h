#ifndef TOML_H
#define TOML_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

#include "errors.h"

namespace toml {

    class Writer {
        std::ofstream file;

        public:
        Writer(std::string file_name);

        void table(std::string value);

        void pair(std::string key, std::string value);
        void pair(std::string key, bool value);
        void new_line();
    };

} // namespace toml

#endif
