#ifndef VRT_PACKET_LOSS_SRC_PROGRAM_ARGUMENTS_H_
#define VRT_PACKET_LOSS_SRC_PROGRAM_ARGUMENTS_H_

#include <filesystem>

namespace vrt::packet_loss {

/**
 * Input arguments to program.
 */
struct ProgramArguments {
    std::filesystem::path file_path_in{};
    std::filesystem::path file_path_out{};
    double                prob_packet_loss{0.0};
    double                prob_burst_loss{0.0};
    bool                  do_byte_swap{false};
};

}  // namespace vrt::packet_loss

#endif
