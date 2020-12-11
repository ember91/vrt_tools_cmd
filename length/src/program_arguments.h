#ifndef VRT_LENGTH_SRC_PROGRAM_ARGUMENTS_H_
#define VRT_LENGTH_SRC_PROGRAM_ARGUMENTS_H_

#include <filesystem>
#include <string>

namespace vrt::length {

/**
 * Input arguments to program.
 */
struct ProgramArguments {
    std::filesystem::path file_path_in{};      /**< Input file path */
    bool                  do_byte_swap{false}; /**< True if byte swap is enabled */
    double                sample_rate{0.0};    /**< Sample rate [Hz] */
};

}  // namespace vrt::length

#endif
