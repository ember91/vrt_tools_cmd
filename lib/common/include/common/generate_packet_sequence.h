#ifndef LIB_COMMON_INCLUDE_COMMON_GENERATE_PACKET_SEQUENCE_H_
#define LIB_COMMON_INCLUDE_COMMON_GENERATE_PACKET_SEQUENCE_H_

#include <cstdint>
#include <filesystem>
#include <functional>

struct vrt_packet;

namespace vrt::common {

void generate_packet_sequence(
    const std::filesystem::path&           file_path,
    vrt_packet*                            p,
    uint64_t                               n,
    const std::function<void(uint64_t i)>& change       = [](uint64_t) {},
    bool                                   do_byte_swap = false);

}  // namespace vrt::common

#endif
