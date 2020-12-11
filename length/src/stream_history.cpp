#include "stream_history.h"

#include <vrt/vrt_types.h>

namespace vrt::length {

/**
 * Update stream history with new packet.
 *
 * \param packet New packet.
 */
void StreamHistory::update(const std::shared_ptr<vrt_packet>& packet) {
    if (packet->header.packet_type == VRT_PT_IF_CONTEXT && packet->if_context.has.sample_rate) {
        // Overwrite previous sample rate, perhaps from command line
        sample_rate_ = packet->if_context.sample_rate;
    }

    if (packet_first_.get() == nullptr) {
        packet_first_ = packet;
    }
    packet_cur_ = packet;

    n_packets_++;
}

}  // namespace vrt::length
