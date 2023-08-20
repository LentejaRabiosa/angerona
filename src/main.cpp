#include <cerrno>
#include <iostream>
#include <stdexcept>

#include "arguments_parser.h"
#include "errors.h"
#include "style.h"

int main(int argc, const char **argv) {
    std::cout << "Running Angerona in " << argv[0] << std::endl;



    arguments_parser::Parser program("aro");
    program.commands_group.set_title(style::set(color::White, decorator::Bold, "Options:"));

    arguments_parser::Parser init("init");
    init.set_description("Createn a new Angerona proyect");
    program.add_subparser(init);

    arguments_parser::Parser compile("compile");
    compile.set_description("Compile the given files");
    compile.add_group("options").set_title(style::set(color::White, decorator::Bold, "Options:"));
    compile.add_argument({"files"}, "options")
        .set_help("Files to compile")
        .remaining();
    program.add_subparser(compile);

    arguments_parser::Parser build("build");
    build.set_description("Build a Angerona proyect");
    program.add_subparser(build);

    arguments_parser::Parser run("run");
    run.set_description("Run the proyect if has been build or compiled");
    program.add_subparser(run);

    if (argc <= 1) {
        program.help();
        return 0;
    }

    try {
        program.parse(argc, argv);
    }
    catch (error::arguments_parser::InvalidArgument err) {
        std::cout
            << style::set(color::Red, decorator::Bold, "error")
            << ": unknown argument: " << err.what()
        << std::endl;

        return 1;
    }
    catch (error::arguments_parser::MissingArguments err) {
        std::cout
            << style::set(color::Red, decorator::Bold, "error")
            << ": " << err.what()
        << std::endl;

        return 1;
    }





    if (program.used_command() == "init") {
        std::cout << "Creating proyect" << std::endl;

    } else if (program.used_command() == "compile") {
        std::cout << "Compiling..." << std::endl;
        for (const auto &file_name : compile.) 
    }





    return 0;
}
