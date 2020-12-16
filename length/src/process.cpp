#include "process.h"

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include <vrt/vrt_types.h>

#include "Progress-CPP/ProgressBar.hpp"
#include "common/comparator_id.h"
#include "common/input_stream.h"
#include "common/packet_id_differences.h"
#include "printer.h"
#include "program_arguments.h"
#include "stream_history.h"

namespace vrt::length {

// For convenience
using PacketPtr        = std::shared_ptr<vrt_packet>;
using StreamHistoryPtr = std::unique_ptr<StreamHistory>;

/**
 * Process file contents.
 *
 * \param args Program arguments.
 *
 * \throw std::runtime_error If there's an error.
 */
void process(const ProgramArguments& args) {
    common::InputStream input_stream(args.file_path_in, args.do_byte_swap);

    std::map<PacketPtr, StreamHistoryPtr, common::ComparatorId> id_streams;

    // Progress bar
    progresscpp::ProgressBar progress(static_cast<uint64_t>(input_stream.get_file_size()), 70);

    // Go over all packets in input file
    uint64_t i{0};
    for (;; ++i) {
        if (!input_stream.read_next_packet()) {
            break;
        }

        // Find Class ID, Stream ID combination in map, or construct new output ID if needed
        PacketPtr packet{input_stream.get_packet()};
        auto      it{id_streams.find(packet)};
        if (it == id_streams.end()) {
            auto pair{id_streams.emplace(packet, std::make_unique<StreamHistory>(args.sample_rate))};

            it = pair.first;
        }

        it->second->update(packet);

        // Handle progress bar
        progress += sizeof(uint32_t) * packet->header.packet_size;
        if (progress.get_ticks() % 65536 == 0) {
            progress.display();
        }
    }

    progress.done();

    // Make vector from map
    std::vector<PacketPtr> v;
    v.reserve(id_streams.size());
    for (const auto& el : id_streams) {
        v.push_back(el.first);
    }

    // Print differences between packets
    common::PacketIdDiffs packet_diffs{common::packet_id_differences(v)};
    for (const auto& el : id_streams) {
        print_difference(*el.second, packet_diffs);
    }

    std::cout.flush();
}

}  // namespace vrt::length
