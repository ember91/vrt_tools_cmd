#include "process.h"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include "vrt/vrt_time.h"
#include "vrt/vrt_types.h"

#include "Progress-CPP/ProgressBar.hpp"
#include "common/comparator_id.h"
#include "common/input_stream.h"
#include "program_arguments.h"

namespace vrt::validate {

// For convenience
using PacketPtr = ::std::shared_ptr<vrt_packet>;
namespace tm    = ::std::chrono;

static bool time_forward(const PacketPtr& pkt_prev, const PacketPtr& pkt) {
    if (pkt_prev->header.tsi == pkt->header.tsi && pkt_prev->header.tsi != VRT_TSI_NONE) {
        if (pkt_prev->fields.integer_seconds_timestamp > pkt->fields.integer_seconds_timestamp) {
            return false;
        } else if (pkt_prev->header.tsf == pkt->header.tsf && pkt_prev->header.tsf != VRT_TSF_NONE &&
                   pkt_prev->fields.integer_seconds_timestamp == pkt->fields.integer_seconds_timestamp &&
                   pkt_prev->fields.fractional_seconds_timestamp > pkt->fields.fractional_seconds_timestamp) {
            return false;
        }
    }

    return true;
}

/**
 * Process file contents.
 *
 * \param args Program arguments.
 *
 * \throw std::runtime_error If there's an error.
 */
void process(const ProgramArguments& args) {
    common::InputStream input_stream(args.file_path_in, args.do_byte_swap, false);

    // Progress bar
    progresscpp::ProgressBar progress(static_cast<uint64_t>(input_stream.get_file_size()), 70);

    double sample_rate{args.sample_rate};

    // Two different ways to store previouse packets: One for the previous in the same stream and one for the previous.
    std::map<PacketPtr, PacketPtr, common::ComparatorId> id_streams;
    PacketPtr                                            pkt_prev;

    // Go over all packets in input file
    uint64_t i{0};
    for (;; ++i) {
        if (!input_stream.read_next_packet()) {
            break;
        }

        // Find Class ID, Stream ID combination in map, or construct new output ID if needed
        PacketPtr pkt{input_stream.get_packet()};

        auto it{id_streams.find(pkt)};
        if (it == id_streams.end()) {
            auto pair{id_streams.emplace(pkt, pkt)};

            it = pair.first;
        }

        if (pkt_prev != nullptr) {
            if (!time_forward(pkt_prev, pkt)) {
                std::cerr << "Time goes backward between packets " << (i - 1) << " and " << i << std::endl;
            }
        }

        pkt_prev = pkt;

        // Handle progress bar
        progress += sizeof(uint32_t) * pkt->header.packet_size;
        if (progress.get_ticks() % 65536 == 0) {
            progress.display();
        }
    }

    progress.done();

    std::cout.flush();
}

}  // namespace vrt::validate
