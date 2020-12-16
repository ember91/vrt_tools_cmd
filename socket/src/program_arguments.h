#ifndef VRT_SOCKET_SRC_PROGRAM_ARGUMENTS_H_
#define VRT_SOCKET_SRC_PROGRAM_ARGUMENTS_H_

#include <filesystem>
#include <string>

namespace vrt::socket {

/**
 * Input arguments to program.
 */
struct ProgramArguments {
    std::filesystem::path file_path_in{};      /**< Input file path */
    std::string           host;                /**< Network host */
    std::string           service;             /**< Network service */
    bool                  do_byte_swap{false}; /**< True if byte swap is enabled */
    double                sample_rate{0.0};    /**< Sample rate [Hz] */
};

}  // namespace vrt::socket

#endif
