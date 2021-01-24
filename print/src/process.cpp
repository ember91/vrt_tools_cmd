#include "process.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "vrt/vrt_types.h"

#include "common/comparator_id.h"
#include "common/input_stream.h"
#include "common/stream_history.h"
#include "program_arguments.h"
#include "stringify.h"
#include "type_printer.h"

namespace vrt::print {

using PacketPtr        = std::shared_ptr<vrt_packet>;
using StreamHistoryPtr = std::unique_ptr<common::StreamHistory>;

/**
 * Process file contents.
 *
 * \param args Program arguments.
 *
 * \throw std::runtime_error If there's an error.
 */
void process(const ProgramArguments& args) {
    common::InputStream input_stream(args.file_path, args.do_byte_swap, false);

    // Number of printed packets
    uint64_t n_printed_packets{0};

    std::map<PacketPtr, StreamHistoryPtr, common::ComparatorId> id_streams;

    // Note that we must go through all packets, since we don't know the size of a packet in the middle of the stream
    // is.
    uint64_t i{0};
    for (; n_printed_packets < args.packet_count; ++i) {
        // Check this after checking EOF
        bool do_print_packet{i >= args.packet_skip};
        if (do_print_packet) {
            n_printed_packets++;
            std::cout << std::string(80, '-') << '\n';

            if (!input_stream.read_next_packet()) {
                break;
            }
        } else {
            if (!input_stream.skip_next_packet()) {
                break;
            }
            continue;
        }

        PacketPtr packet{input_stream.get_packet()};

        WriteCols("#", std::to_string(i));
        print_header(*packet);
        print_fields(*packet, args.sample_rate);
        print_body(*packet);

        // Get sample rate
        double sample_rate{args.sample_rate};
        auto   it{id_streams.find(packet)};
        if (it == id_streams.end()) {
            id_streams[packet] = std::make_unique<common::StreamHistory>(args.sample_rate);
        } else {
            it->second->update(packet);
            sample_rate = it->second->get_sample_rate();
        }

        if (packet->header.packet_type == VRT_PT_IF_CONTEXT) {
            print_if_context(*packet, sample_rate);
        }
        if (packet->header.has.trailer) {
            print_trailer(*packet);
        }
    }

    // Print some warnings if not all packets were printed
    if (i != 0) {
        if (n_printed_packets == 0) {
            std::cerr << "Warning: Skipped over all " << i << " packet(s)\n";
        } else if (n_printed_packets < args.packet_count && args.packet_count != static_cast<uint64_t>(-1)) {
            std::cerr << "Warning: Printed only " << n_printed_packets << " out of " << args.packet_count
                      << " packet(s) due to end of file\n";
        }
    }

    // Ensure text is output
    std::cout << std::flush;
}

}  // namespace vrt::print
