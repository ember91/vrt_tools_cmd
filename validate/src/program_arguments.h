#ifndef VRT_VALIDATE_SRC_PROGRAM_ARGUMENTS_H_
#define VRT_VALIDATE_SRC_PROGRAM_ARGUMENTS_H_

#include <filesystem>
#include <string>

namespace vrt::validate {

enum class protocol_type { UDP, TCP };

/**
 * Input arguments to program.
 */
struct ProgramArguments {
    std::filesystem::path file_path_in{};      /**< Input file path */
    bool                  do_byte_swap{false}; /**< True if byte swap is enabled */
    double                sample_rate{0.0};    /**< Sample rate [Hz] */
};

}  // namespace vrt::validate

#endif
