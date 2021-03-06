#ifndef VRT_SPLIT_SRC_PROGRAM_ARGUMENTS_H_
#define VRT_SPLIT_SRC_PROGRAM_ARGUMENTS_H_

#include <filesystem>

namespace vrt::split {

/**
 * Input arguments to program.
 */
struct ProgramArguments {
    std::filesystem::path file_path_in{};
    bool                  do_byte_swap{false};
};

}  // namespace vrt::split

#endif
