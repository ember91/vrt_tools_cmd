#include "process.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>

#include "Progress-CPP/ProgressBar.hpp"
#include "common/input_stream.h"
#include "common/output_stream.h"
#include "program_arguments.h"

namespace vrt::merge {

// For convenience
using InputStreamPtr = std::shared_ptr<InputStream>;

/**
 * Comparator for packets by time.
 */
struct ComparatorTime {
    /**
     * Compare packets by time stamps.
     *
     * \param a Packet 1.
     * \param b Packet 2.
     *
     * \return True if a is later than b.
     */
    bool operator()(const InputStreamPtr& a, const InputStreamPtr& b) const {
        // Get headers and fields
        const vrt_header& ah{a->get_packet()->header};
        const vrt_header& bh{b->get_packet()->header};
        const vrt_fields& af{a->get_packet()->fields};
        const vrt_fields& bf{b->get_packet()->fields};

        if (ah.tsi == VRT_TSI_NONE) {
            std::stringstream ss;
            ss << "Packet in " << a->get_file_path() << ": Integer second timestamp is NONE";
            throw std::runtime_error(ss.str());
        }

        if (ah.tsi != bh.tsi) {
            std::stringstream ss;
            ss << "Cannot compare different Integer second timestamps (TSI) of " << vrt_string_tsi(ah.tsi) << " with "
               << vrt_string_tsi(bh.tsi);
            throw std::runtime_error(ss.str());
        }

        if (af.integer_seconds_timestamp < bf.integer_seconds_timestamp) {
            return false;
        }
        if (af.integer_seconds_timestamp > bf.integer_seconds_timestamp) {
            return true;
        }

        if (ah.tsf != bh.tsf) {
            std::stringstream ss;
            ss << "Cannot compare different Fractional second timestamps (TSF) of " << vrt_string_tsf(ah.tsf)
               << " with " << vrt_string_tsf(bh.tsf);
            throw std::runtime_error(ss.str());
        }

        if (ah.tsf == VRT_TSF_NONE) {
            // Equality
            return false;
        }

        if (af.fractional_seconds_timestamp > bf.fractional_seconds_timestamp) {
            return true;
        }

        // a <= b
        return false;
    }
};

/**
 * Process file contents.
 *
 * \param args Program arguments.
 *
 * \throw std::runtime_error If there's an error.
 */
void process(const ProgramArguments& args) {
    OutputStream output_stream(args.file_path_out);

    // Earliest element is on top
    std::priority_queue<InputStreamPtr, std::vector<InputStreamPtr>, ComparatorTime> input_stream_queue;

    // Start by filling queue
    std::streampos total_file_size_bytes{0};
    for (const auto& file_path_in : args.file_paths_in) {
        InputStreamPtr stream{std::make_shared<InputStream>(file_path_in, args.do_byte_swap)};
        if (stream->read_next_packet()) {
            input_stream_queue.push(stream);
        }
        total_file_size_bytes += stream->get_file_size();
    }

    progresscpp::ProgressBar progress(static_cast<size_t>(total_file_size_bytes), 70);

    try {
        // Loop until there are no more packets left in any input file
        while (!input_stream_queue.empty()) {
            // Get earliest input packet from queue
            InputStreamPtr input_stream{input_stream_queue.top()};
            input_stream_queue.pop();

            // Write input packet to output
            input_stream->write(output_stream.get_file());

            // Read next packet and insert at the right place into queue if any left
            if (input_stream->read_next_packet()) {
                input_stream_queue.push(input_stream);
            }

            // Handle progress bar
            progress += sizeof(uint32_t) * input_stream->get_packet()->header.packet_size;
            if (progress.get_ticks() % 65536 == 0) {
                progress.display();
            }
        }

        progress.done();
    } catch (...) {
        // Cleanup and rethrow
        output_stream.remove_file();
        throw;
    }
}

}  // namespace vrt::merge
