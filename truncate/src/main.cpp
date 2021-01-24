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

namespace fs = std::filesystem;

/**
 * Setup program command line argument parsing.
 *
 * \param app CLI11 app.
 *
 * \return Program input arguments.
 */
static vrt::truncate::ProgramArguments setup_arg_parse(CLI::App* app) {
    vrt::truncate::ProgramArguments args;

    // Input file
    CLI::Option* opt_file_in{app->add_option("-i,--input-file", args.file_path_in, "Input file path")};
    opt_file_in->required(true);
    opt_file_in->check(CLI::ExistingFile);

    // Output file
    CLI::Option* opt_file_out{app->add_option("-o,--output-file", args.file_path_out, "Output file path")};
    opt_file_out->required(true);

    // Byte swap
    app->add_flag("-b,--byte-swap", args.do_byte_swap,
                  "Apply byte swap before parsing file. Note that this will NOT byte swap packet output.");

    // Begin
    app->add_option("-B,--begin", args.begin, "Index of first packet to keep (inclusive)");

    // End
    app->add_option("-e,--end", args.end, "Index of last packet to keep (exclusive)");

    // Count
    app->add_option("-c,--count", args.count, "Number of packets to keep");

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
    CLI::App                        app(CMAKE_PROJECT_DESCRIPTION, CMAKE_PROJECT_NAME);
    vrt::truncate::ProgramArguments program_args{setup_arg_parse(&app)};
    CLI11_PARSE(app, argc, argv)

    // Parameter validation
    try {
        if (fs::equivalent(program_args.file_path_in, program_args.file_path_out)) {
            std::cerr << "Cannot use the same input as output file path: " << program_args.file_path_in << std::endl;
            return EXIT_FAILURE;
        }
    } catch (const fs::filesystem_error&) {
        // Do nothing. Output path does not exist. If input path doesn't exist it will be shown when file opens
        // anyway.
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

    // Check that begin, end, and count play together
    bool set_begin{program_args.begin != 0};
    bool set_end{program_args.end != std::numeric_limits<uint64_t>::max()};
    bool set_count{program_args.count != std::numeric_limits<uint64_t>::max()};
    if (set_begin && set_end && set_count) {
        std::cerr << "Cannot set first packet, last packet and number of packets at the same time" << std::endl;
        return EXIT_FAILURE;
    }
    if (!set_begin && !set_end && !set_count) {
        std::cerr << "Supply begin, end or, count. Supplying neither is pointless." << std::endl;
        return EXIT_FAILURE;
    }

    // Ensure end is after begin
    if (set_begin && set_end && program_args.end < program_args.begin) {
        std::cerr << "Begin (" << program_args.begin << ") must be after end (" << program_args.end << ")" << std::endl;
        return EXIT_FAILURE;
    }

    // Ensure count isn't too large
    if (set_end && set_count && program_args.count > program_args.end) {
        std::cerr << "Count (" << program_args.count << ") cannot be greater than end (" << program_args.end << ")"
                  << std::endl;
        return EXIT_FAILURE;
    }

    // Process
    try {
        vrt::truncate::Processor processor(program_args);
        processor.process();
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
