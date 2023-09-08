#include "errors.h"

using namespace error;


/*
 *  Invalid Argument
 */
arguments_parser::InvalidArgument::InvalidArgument(std::string_view name) : error(initError(name)) {}
std::string arguments_parser::InvalidArgument::initError(std::string_view name) {
    return style::error()
        + " unknown argument name: "
        + name.data();
}
const char* arguments_parser::InvalidArgument::what() {
    return error.data();
}






/*
 *  Missing Arguments
 */
arguments_parser::MissingArguments::MissingArguments() :
    error(style::error()
        + " missing arguments")
{}
const char* arguments_parser::MissingArguments::what() {
    return error.data();
}




/*
 *  Too Few Arguments
 */
arguments_parser::TooFewArguments::TooFewArguments() :
    error(style::error()
        + " too few arguments")
{}
const char* arguments_parser::TooFewArguments::what() {
    return error.data();
}









/*
 *  Unknown File Extension
 */
files::UnknownFileExtension::UnknownFileExtension(std::string_view name) : error(initError(name)) {}
std::string files::UnknownFileExtension::initError(std::string_view name) {
    return style::error()
        + " unknown file extension: "
        + name.data();
}
const char* files::UnknownFileExtension::what() {
    return error.data();
}





/*
 *  Duplicated File Name
 */
files::DuplicatedFileName::DuplicatedFileName(std::string_view name) :
    error(initError(name))
{}
std::string files::DuplicatedFileName::initError(std::string_view name) {
    return style::error()
        + " duplicated file name: "
        + name.data();
}
const char* files::DuplicatedFileName::what() {
    return error.data();
}





/*
 *  Could Not Open File
 */
files::CouldNotOpenFile::CouldNotOpenFile(std::string_view name) :
    error(initError(name))
{}
std::string files::CouldNotOpenFile::initError(std::string_view name) {
    return style::error()
        + " could not open file: "
        + name.data();
}
const char* files::CouldNotOpenFile::what() {
    return error.data();
}








/*
 *  Duplicated Keyword
 */
parser::DuplicatedKeyword::DuplicatedKeyword(std::string_view name) :
    error(initError(name))
{}
std::string parser::DuplicatedKeyword::initError(std::string_view name) {
    return style::error()
        + " duplicated keyword: `"
        + name.data()
        + "`";
}
const char* parser::DuplicatedKeyword::what() {
    return error.data();
}




/*
 *  Bad Keywords Order
 */
parser::BadKeywordsOrder::BadKeywordsOrder(std::string_view kw1, std::string_view kw2) :
    error(initError(kw1, kw2))
{}
std::string parser::BadKeywordsOrder::initError(std::string_view kw1, std::string_view kw2) {
    return style::error()
        + " `"
        + kw1.data()
        + "`"
        + " can't be before `"
        + kw2.data()
        + "`";
}
const char* parser::BadKeywordsOrder::what() {
    return error.data();
}






/*
 *  Invalid Name
 */
parser::InvalidName::InvalidName(std::string_view name) :
    error(initError(name))
{}
std::string parser::InvalidName::initError(std::string_view name) {
    return style::error()
        + " invalid name: "
        + name.data();
}
const char* parser::InvalidName::what() {
    return error.data();
}
