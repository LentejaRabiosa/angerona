#include <cerrno>
#include <iostream>
#include <stdexcept>

#include "arguments_parser.h"
#include "errors.h"
#include "style.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, const char **argv) {
    std::cout << "Running Angerona in " << argv[0] << std::endl;



    arguments_parser::Parser program("aro");

    arguments_parser::Parser init("init");
    init.description = "Createn a new Angerona proyect";
    program.add_subparser(init);

    arguments_parser::Parser compile("compile");
    compile.description = "Compile the given files";
    compile.add_group("options").title = "Options:";
    compile.add_argument({"files..."}, "options")
        .set_help("Files to compile")
        .remaining();
    compile.add_argument({"-o", "--output"}, "options")
        .set_help("Output file");
    program.add_subparser(compile);

    arguments_parser::Parser build("build");
    build.description = "Build a Angerona proyect";
    program.add_subparser(build);

    arguments_parser::Parser run("run");
    run.description = "Run the proyect if has been build or compiled";
    program.add_subparser(run);

    arguments_parser::Parser lexer("lexer");
    lexer.description = "Generates a file with the lexer output of a given file";
    lexer.add_group("options").title = "Options:";
    lexer.add_argument({"file"}, "options")
        .set_help("File to process");
    lexer.add_argument({"-o", "--output"}, "options")
        .set_help("Output file name");
    program.add_subparser(lexer);

    if (argc <= 1) {
        program.help();
        return 0;
    }

    try {
        program.parse(argc, argv);
    }
    catch (error::arguments_parser::InvalidArgument err) {
        std::cout << err.what() << std::endl;
        return 1;
    }
    catch (error::arguments_parser::MissingArguments err) {
        std::cout << err.what() << std::endl;
        return 1;
    }
    catch (error::arguments_parser::TooFewArguments err) {
        std::cout << err.what() << std::endl;
        return 1;
    }
    catch (error::arguments_parser::HelpAbort) {}





    try {
        if (init.is_parsed) {
            std::cout << "Creating proyect" << std::endl;

        } else if (compile.is_parsed) {
            std::cout << "Compiling..." << std::endl;
            auto file_names = compile.get_values("files...");

            try {
                parser::FilesParser files_parser(file_names);
            }
            catch (error::files::UnknownFileExtension err) {
                std::cout << err.what() << std::endl;
                return 1;
            }
            catch (error::parser::DuplicatedKeyword err) {
                std::cout << err.what() << std::endl;
                return 1;
            }
            catch (error::parser::BadKeywordsOrder err) {
                std::cout << err.what() << std::endl;
                return 1;
            }
            catch (error::parser::InvalidName err) {
                std::cout << err.what() << std::endl;
                return 1;
            }

        } else if (lexer.is_parsed) {
            std::cout << "Running the lexer..." << std::endl;

            auto file_name = lexer.get_values("file")[0];
            std::ifstream file(file_name);
            if (!file.is_open()) throw error::files::CouldNotOpenFile(file_name.data());

            auto lx = lexer::Lexer(file);
            std::string output_name;
            if (lexer.is_used("-o")) {
                output_name = lexer.get_values("-o")[0];
            } else {
                output_name = files::name(file_name) + ".lexer";
            }

            lx.to_file(output_name);
        }
    } // try
    catch (error::files::CouldNotOpenFile err) {
        std::cout << err.what() << std::endl;
        return 1;
    }





    return 0;
}
