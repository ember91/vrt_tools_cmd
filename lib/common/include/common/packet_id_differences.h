#ifndef LIB_COMMON_INCLUDE_COMMON_PACKET_ID_DIFFERENCES_H_
#define LIB_COMMON_INCLUDE_COMMON_PACKET_ID_DIFFERENCES_H_

#include <memory>
#include <vector>

struct vrt_packet;

namespace vrt {

/**
 * Describe differences between packets, as booleans.
 */
struct PacketIdDiffs {
    bool any_has_class_id{false};   // If any packet has Class ID
    bool any_has_stream_id{false};  // If any packet has Stream ID
    bool diff_oui{false};           // If OUI differs
    bool diff_icc{false};           // If Information class code differs
    bool diff_pcc{false};           // If Packet class code differs
    bool diff_sid{false};           // If Stream ID differs
};

PacketIdDiffs packet_id_differences(const std::vector<std::shared_ptr<vrt_packet>>& packets);

}  // namespace vrt

#endif
