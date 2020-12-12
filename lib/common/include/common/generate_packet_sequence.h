#ifndef LIB_COMMON_INCLUDE_COMMON_GENERATE_PACKET_SEQUENCE_H_
#define LIB_COMMON_INCLUDE_COMMON_GENERATE_PACKET_SEQUENCE_H_

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>

struct vrt_packet;

namespace vrt {

void generate_packet_sequence(
    const std::filesystem::path&                        file_path,
    vrt_packet*                                         p,
    size_t                                              n,
    const std::function<void(uint64_t i, vrt_packet*)>& change       = [](uint64_t i, vrt_packet*) {},
    bool                                                do_byte_swap = false);

}  // namespace vrt

#endif
