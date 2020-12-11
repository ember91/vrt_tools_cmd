#ifndef VRT_MERGE_SRC_PROGRAM_ARGUMENTS_H_
#define VRT_MERGE_SRC_PROGRAM_ARGUMENTS_H_

#include <filesystem>
#include <string>
#include <vector>

namespace vrt::merge {

/**
 * Input arguments to program.
 */
struct ProgramArguments {
    std::vector<std::filesystem::path> file_paths_in{};
    std::filesystem::path              file_path_out{};
    bool                               do_byte_swap{false};
};

}  // namespace vrt::merge

#endif
