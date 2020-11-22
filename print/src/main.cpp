#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include <vrt/vrt_util.h>

#include "CLI11.hpp"

#include "process.h"
#include "program_arguments.h"

/**
 * Parse and validate program arguments from command line.
 *
 * \param argc Number of input arguments.
 * \param argv Input arguments [argc].
 *
 * \return Parse arguments.
 */
static vrt::print::ProgramArguments parse_arguments(int argc, const char** argv) {
    vrt::print::ProgramArguments args;

    CLI::App app(
        "Prints information in vita49 VRT format files in a human readable format. Only prints, does not "
        "validate packets.",
        "vrt_print");

    CLI::Option* opt_file{app.add_option("-f,--file,file", args.file_path, "Input file path")};
    opt_file->required(true);
    opt_file->check(CLI::ExistingFile);

    CLI::Option* opt_packet_skip{
        app.add_option("-s,--packet_skip", args.packet_skip, "Number of packets to skip before starting to print")};
    opt_packet_skip->check(CLI::NonNegativeNumber);
    // Put prefixes in reverse order, so they are shown in ascending order.
    opt_packet_skip->transform(CLI::AsNumberWithUnit(
        std::map<std::string, std::size_t>{{"T", 1000000000000}, {"G", 1000000000}, {"M", 1000000}, {"k", 1000}},
        CLI::AsNumberWithUnit::CASE_SENSITIVE));

    args.packet_count = static_cast<size_t>(-1);
    CLI::Option* opt_packet_count{app.add_option("-c,--packet_count", args.packet_count, "Number of packets to print")};
    opt_packet_count->check(CLI::PositiveNumber);
    opt_packet_count->transform(CLI::AsNumberWithUnit(
        std::map<std::string, std::size_t>{{"T", 1000000000000}, {"G", 1000000000}, {"M", 1000000}, {"k", 1000}},
        CLI::AsNumberWithUnit::CASE_SENSITIVE));

    app.add_flag("-b,--byte_swap", args.do_byte_swap, "Apply byte swap before parsing file");

    app.parse(argc, argv);

    // Check that endianness of platform compared to byte swap parameter makes sense
    if (vrt_is_platform_little_endian() && !args.do_byte_swap) {
        std::cerr << "Warning: Detected little endian platform, but byte swap is NOT enabled. This will only work on "
                     "non-conforming VRT packets."
                  << std::endl;
    } else if (args.do_byte_swap) {
        std::cerr << "Warning: Detected big endian platform, but byte swap IS enabled. This will only work on "
                     "non-conforming VRT packets."
                  << std::endl;
    }

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

    try {
        // Parse arguments
        vrt::print::ProgramArguments args{parse_arguments(argc, argv)};
        // Process
        vrt::print::process(args);
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
