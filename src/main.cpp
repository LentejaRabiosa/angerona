#include <cerrno>
#include <iostream>
#include <stdexcept>

#include "arguments_parser.h"
#include "errors.h"
#include "style.h"
#include "lexer.h"

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
    program.add_subparser(compile);

    arguments_parser::Parser build("build");
    build.description = "Build a Angerona proyect";
    program.add_subparser(build);

    arguments_parser::Parser run("run");
    run.description = "Run the proyect if has been build or compiled";
    program.add_subparser(run);

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
    catch (error::arguments_parser::HelpAbort) {}





    if (init.is_parsed) {
        std::cout << "Creating proyect" << std::endl;

    } else if (compile.is_parsed) {
        std::cout << "Compiling..." << std::endl;
        auto file_names = compile.get_values("files...");

        try {
            lexer::Lexer l(file_names);
            auto item = l.consume("caca.aro");
        }
        catch (error::lexer::UnknownFileExtension err) {
            std::cout << err.what() << std::endl;
            return 1;
        }
    }





    return 0;
}
