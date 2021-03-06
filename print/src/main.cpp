#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

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
static vrt::print::ProgramArguments setup_arg_parse(CLI::App* app) {
    vrt::print::ProgramArguments args;

    CLI::Option* opt_file{app->add_option("-f,--file,file", args.file_path, "Input file path")};
    opt_file->required(true);
    opt_file->check(CLI::ExistingFile);

    // Sample rate
    CLI::Option* opt_sample_rate{app->add_option(
        "-s,--sample-rate", args.sample_rate,
        "Sample rate [Hz]. If IF context sample rate appears in the stream it will take precedence over this option.")};
    opt_sample_rate->check(CLI::NonNegativeNumber);
    opt_sample_rate->transform(CLI::AsNumberWithUnit(
        std::map<std::string, uint64_t>{{"T", 1000000000000}, {"G", 1000000000}, {"M", 1000000}, {"k", 1000}},
        CLI::AsNumberWithUnit::CASE_SENSITIVE));

    // Packet skip
    CLI::Option* opt_packet_skip{app->add_option(
        "-S,--packet-skip", args.packet_skip,
        "Number of packets to skip before starting to print. Supports prefixes such as k, M, G, and T.")};
    opt_packet_skip->check(CLI::NonNegativeNumber);
    // Put prefixes in reverse order, so they are shown in ascending order.
    opt_packet_skip->transform(CLI::AsNumberWithUnit(
        std::map<std::string, uint64_t>{{"T", 1000000000000}, {"G", 1000000000}, {"M", 1000000}, {"k", 1000}},
        CLI::AsNumberWithUnit::CASE_SENSITIVE));

    // Packet count
    args.packet_count = static_cast<uint64_t>(-1);
    CLI::Option* opt_packet_count{
        app->add_option("-c,--packet-count", args.packet_count,
                        "Number of packets to print. Supports prefixes such as k, M, G, and T.")};
    opt_packet_count->check(CLI::PositiveNumber);
    opt_packet_count->transform(CLI::AsNumberWithUnit(
        std::map<std::string, uint64_t>{{"T", 1000000000000}, {"G", 1000000000}, {"M", 1000000}, {"k", 1000}},
        CLI::AsNumberWithUnit::CASE_SENSITIVE));

    // Byte swap
    app->add_flag("-b,--byte-swap", args.do_byte_swap, "Apply byte swap before parsing file");

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
    // Overall design: Print information directly when it's available. This makes debugging broken packet streams
    // easier. Flush only when exiting program for performance.

    // Speed up output
    std::ios_base::sync_with_stdio(false);

    CLI::App app(CMAKE_PROJECT_DESCRIPTION, CMAKE_PROJECT_NAME);

    vrt::print::ProgramArguments program_args{setup_arg_parse(&app)};
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

    try {
        vrt::print::process(program_args);
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
