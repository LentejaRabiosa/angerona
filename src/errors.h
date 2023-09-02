#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <cstring>

#include "style.h"

namespace error {

    namespace arguments_parser {

        class InvalidArgument : std::exception {
            std::string error;
            static std::string initError(std::string_view name);

            public:
            InvalidArgument(std::string_view name);
            const char* what();
        };

        class MissingArguments : std::exception {
            std::string error;

            public:
            MissingArguments();
            const char* what();
        };

        class TooFewArguments : std::exception {
            std::string error;

            public:
            TooFewArguments();
            const char* what();
        };

        class HelpAbort : std::exception {};

    } // namespace arguments_parse

    namespace files {

        class UnknownFileExtension : std::exception {
            std::string error;
            static std::string initError(std::string_view name);

            public:
            UnknownFileExtension(std::string_view extension);
            const char* what();
        };

        class DuplicatedFileName : std::exception {
            std::string error;
            static std::string initError(std::string_view name);

            public:
            DuplicatedFileName(std::string_view name);
            const char* what();
        };

        class CouldNotOpenFile : std::exception {
            std::string error;
            static std::string initError(std::string_view name);

            public:
            CouldNotOpenFile(std::string_view name);
            const char* what();
        };

    } // namespace files

    namespace parser {

        class DuplicatedKeyword : std::exception {
            std::string error;
            static std::string initError(std::string_view name);

            public:
            DuplicatedKeyword(std::string_view name);
            const char* what();
        };

        class BadKeywordsOrder : std::exception {
            std::string error;
            static std::string initError(std::string_view kw1, std::string_view kw2);

            public:
            BadKeywordsOrder(std::string_view kw1, std::string_view kw2);
            const char* what();
        };

    } // namespace parser

} // namespace errors

#endif
