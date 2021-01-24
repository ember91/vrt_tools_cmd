#ifndef VRT_TRUNCATE_SRC_PROGRAM_ARGUMENTS_H_
#define VRT_TRUNCATE_SRC_PROGRAM_ARGUMENTS_H_

#include <filesystem>

namespace vrt::truncate {

/**
 * Input arguments to program.
 */
struct ProgramArguments {
    std::filesystem::path file_path_in{};
    std::filesystem::path file_path_out{};
    uint64_t              begin{0};
    uint64_t              end{std::numeric_limits<uint64_t>::max()};
    uint64_t              count{std::numeric_limits<uint64_t>::max()};
    bool                  do_byte_swap{false};
};

}  // namespace vrt::truncate

#endif
