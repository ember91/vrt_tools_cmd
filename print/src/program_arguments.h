#ifndef VRT_PRINT_SRC_PROGRAM_ARGUMENTS_H_
#define VRT_PRINT_SRC_PROGRAM_ARGUMENTS_H_

#include <filesystem>
#include <string>

namespace vrt::print {

/**
 * Input arguments to program.
 */
struct ProgramArguments {
    std::filesystem::path file_path{};
    uint64_t              packet_skip{0};
    uint64_t              packet_count{static_cast<uint64_t>(-1)};
    bool                  do_byte_swap{false};
};

}  // namespace vrt::print

#endif
