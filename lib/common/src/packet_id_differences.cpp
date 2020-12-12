#include "common/packet_id_differences.h"

#include <memory>
#include <vector>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

namespace vrt {

/**
 * Find differences between packets.
 *
 * \param packets List of packets.
 * \return booleans describing packet differences.
 */
PacketIdDiffs packet_id_differences(const std::vector<std::shared_ptr<vrt_packet>>& packets) {
    // Pointers to previous packets
    vrt_packet* prev_p{nullptr};    // Previous packet
    vrt_packet* prev_cid{nullptr};  // Previous packet with Class ID
    vrt_packet* prev_sid{nullptr};  // Previous packet with Stream ID

    // Calculate
    PacketIdDiffs ret;
    for (const auto& p : packets) {
        if (prev_p != nullptr) {
            if (p->header.has.class_id) {
                ret.any_has_class_id = true;
            }
            if (vrt_has_stream_id(&p->header)) {
                ret.any_has_stream_id = true;
            }
            if (prev_cid != nullptr && p->header.has.class_id) {
                if (p->fields.class_id.oui != prev_cid->fields.class_id.oui) {
                    ret.diff_oui = true;
                }
                if (p->fields.class_id.information_class_code != prev_cid->fields.class_id.information_class_code) {
                    ret.diff_icc = true;
                }
                if (p->fields.class_id.packet_class_code != prev_cid->fields.class_id.packet_class_code) {
                    ret.diff_pcc = true;
                }
            }
            if (prev_sid != nullptr && vrt_has_stream_id(&p->header)) {
                if (p->fields.stream_id != prev_sid->fields.stream_id) {
                    ret.diff_sid = true;
                }
            }
        }

        // Save pointers to previous packets
        prev_p = p.get();
        if (p->header.has.class_id) {
            prev_cid = p.get();
        }
        if (vrt_has_stream_id(&p->header)) {
            prev_sid = p.get();
        }
    }

    return ret;
}

}  // namespace vrt
