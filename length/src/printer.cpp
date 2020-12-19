#include "printer.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>

#include <vrt/vrt_time.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

#include "common/packet_id_differences.h"
#include "stream_history.h"

namespace vrt::length {

/**
 * Print Class and Stream ID information.
 *
 * \param p A packet in stream.
 */
static void print_ids(vrt_packet* p, const common::PacketIdDiffs& differences) {
    std::cout << std::hex << std::setfill('0');
    if (p->header.has.class_id) {
        if (differences.diff_oui || differences.diff_icc || differences.diff_pcc) {
            std::cout << "Class ID\n";
            if (differences.diff_oui) {
                std::cout << "  OUI: 0x" << std::setw(6) << p->fields.class_id.oui << '\n';
            }
            if (differences.diff_icc) {
                std::cout << "  Information class code: 0x" << std::setw(4) << p->fields.class_id.information_class_code
                          << '\n';
            }
            if (differences.diff_pcc) {
                std::cout << "  Packet class code: 0x" << std::setw(4) << p->fields.class_id.packet_class_code << '\n';
            }
        }
    } else if (differences.any_has_class_id) {
        std::cout << "Class ID: None\n";
    }
    if (vrt_has_stream_id(&p->header)) {
        if (differences.diff_sid) {
            std::cout << "Stream ID: 0x" << std::setw(8) << p->fields.stream_id << '\n';
        }
    } else if (differences.any_has_stream_id) {
        std::cout << "Stream ID: None\n";
    }

    // Reset stream manipulators
    std::cout << std::setw(0) << std::dec << std::setfill(' ');
}

/**
 * Print differences between first and last packets in a specific stream.
 *
 * \param stream_history Stream history.
 * \param differences    Differences between streams.
 */
void print_difference(const StreamHistory& stream_history, const common::PacketIdDiffs& differences) {
    vrt_packet* fir{stream_history.get_packet_first().get()};
    vrt_packet* cur{stream_history.get_packet_current().get()};
    double      sample_rate{stream_history.get_sample_rate()};

    print_ids(fir, differences);

    std::cout << "Number of packets: " << stream_history.get_number_of_packets() << '\n';

    vrt_time diff;
    int      rv{vrt_time_difference(cur, fir, sample_rate, &diff)};
    if (rv == 0) {
        std::cout << "Time difference: " << diff.s << '.' << std::setfill('0') << std::setw(11) << diff.ps << " s\n";

        // Reset stream manipulators
        std::cout << std::setw(0) << std::setfill(' ');
    }
}

}  // namespace vrt::length
