#ifndef VRT_TOOLS_CMD_LIB_GENERATE_PACKET_SEQUENCE_H_
#define VRT_TOOLS_CMD_LIB_GENERATE_PACKET_SEQUENCE_H_

#include <filesystem>
#include <functional>

#include <vrt/vrt_types.h>

namespace vrt {

void generate_packet_sequence(
    const std::filesystem::path&                        file_path,
    vrt_packet*                                         p,
    size_t                                              n,
    const std::function<void(uint64_t i, vrt_packet*)>& change       = [](uint64_t i, vrt_packet*) {},
    bool                                                do_byte_swap = false);

}  // namespace vrt

#endif
