#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "vrt/vrt_util.h"

#include "CLI/CLI.hpp"

#include "process.h"
#include "program_arguments.h"

#ifndef CMAKE_PROJECT_NAME
#error "No project name definition from CMake"
#endif
#ifndef CMAKE_PROJECT_DESCRIPTION
#error "No project definition from CMake"
#endif

/**
 * Setup program command line argument parsing.
 *
 * \param app CLI11 app.
 *
 * \return Program input arguments.
 */
static vrt::length::ProgramArguments setup_arg_parse(CLI::App* app) {
    vrt::length::ProgramArguments args;

    // File
    CLI::Option* opt_file_in{app->add_option("-f,--file,file", args.file_path_in, "File path")};
    opt_file_in->required(true);
    opt_file_in->check(CLI::ExistingFile);

    // Byte swap
    app->add_flag("-b,--byte-swap", args.do_byte_swap, "Apply byte swap before parsing file");

    // Sample rate
    CLI::Option* opt_sample_rate{app->add_option(
        "-s,--sample-rate", args.sample_rate,
        "Sample rate [Hz]. If IF context sample rate appears in the stream it will take precedence over this option.")};
    opt_sample_rate->check(CLI::NonNegativeNumber);
    opt_sample_rate->transform(CLI::AsNumberWithUnit(
        std::map<std::string, uint64_t>{{"T", 1000000000000}, {"G", 1000000000}, {"M", 1000000}, {"k", 1000}},
        CLI::AsNumberWithUnit::CASE_SENSITIVE));

    return args;
}

/**
 * Starting point.
 *
 * \param argc Number of input arguments.
 * \param argv Input arguments [argc].
 *
 * \return EXIT_SUCCESS if success, and EXIT_FAILURE otherwise.
 */
int main(int argc, const char** argv) {
    // Parse arguments
    CLI::App                      app(CMAKE_PROJECT_DESCRIPTION, CMAKE_PROJECT_NAME);
    vrt::length::ProgramArguments program_args{setup_arg_parse(&app)};
    CLI11_PARSE(app, argc, argv)

    // Check that endianness of platform compared to byte swap parameter makes sense
    if (vrt_is_platform_little_endian() && !program_args.do_byte_swap) {
        std::cerr << "Warning: Detected little endian platform, but byte swap is NOT enabled. This will only work on "
                     "non-conforming VRT packets."
                  << std::endl;
    } else if (program_args.do_byte_swap) {
        std::cerr << "Warning: Detected big endian platform, but byte swap IS enabled. This will only work on "
                     "non-conforming VRT packets."
                  << std::endl;
    }

    // Process
    try {
        vrt::length::process(program_args);
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
