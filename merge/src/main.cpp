#include <cstdlib>
#include <filesystem>
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

namespace fs = ::std::filesystem;

/**
 * Setup program command line argument parsing.
 *
 * \param app CLI11 app.
 *
 * \return Program input arguments.
 */
static vrt::merge::ProgramArguments setup_arg_parse(CLI::App* app) {
    vrt::merge::ProgramArguments args;

    // Input files
    CLI::Option* opt_files_in{app->add_option("-i,--input-files", args.file_paths_in, "Input file path(s)")};
    opt_files_in->required(true);
    opt_files_in->check(CLI::ExistingFile);

    // Output file
    CLI::Option* opt_file_out{app->add_option("-o,--output-file", args.file_path_out, "Output file path")};
    opt_file_out->required(true);

    // Byte swap
    app->add_flag("-b,--byte-swap", args.do_byte_swap,
                  "Apply byte swap before parsing file. Note that this will NOT byte swap packet output.");

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
    CLI::App                     app(CMAKE_PROJECT_DESCRIPTION, CMAKE_PROJECT_NAME);
    vrt::merge::ProgramArguments program_args{setup_arg_parse(&app)};
    CLI11_PARSE(app, argc, argv)

    // Parameter validation
    if (program_args.file_paths_in.empty()) {
        std::cerr << "--input_files is required with at least one file" << std::endl;
        return EXIT_FAILURE;
    }
    for (const fs::path& path_in : program_args.file_paths_in) {
        try {
            if (fs::equivalent(path_in, program_args.file_path_out)) {
                std::cerr << "Cannot use the same input as output file path: " << path_in << std::endl;
                return EXIT_FAILURE;
            }
        } catch (const fs::filesystem_error&) {
            // Do nothing. Output path does not exist. If input path doesn't exist it will be shown when file opens
            // anyway.
        }
    }

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
        vrt::merge::process(program_args);
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
