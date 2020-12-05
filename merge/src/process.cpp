#include "process.h"

#include <algorithm>
#include <csignal>
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
#include "byte_swap.h"
#include "input_stream.h"
#include "program_arguments.h"

namespace fs = std::filesystem;

namespace vrt::merge {

// For convenience
using InputStreamPtr = std::shared_ptr<InputStream>;

// Static so they cam be used when signal is called
static fs::path      file_path_out;
static std::ofstream file_out;

/**
 * Close output file stream and remove any traces.
 */
static void remove_output_file() {
    file_out.close();
    try {
        fs::remove(file_path_out);
    } catch (const fs::filesystem_error&) {
        // Do nothing
    }
}

/**
 * Handle shutdown signals gracefully by closing and removing any output file before shutdown.
 *
 * \param signum Signal number.
 */
[[noreturn]] static void signal_handler(int signum) {
    remove_output_file();

    std::exit(signum);
}

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
        const vrt_header& ah{a->get_header()};
        const vrt_header& bh{b->get_header()};
        const vrt_fields& af{a->get_fields()};
        const vrt_fields& bf{b->get_fields()};

        if (ah.tsi == VRT_TSI_NONE) {
            std::stringstream ss;
            ss << "Packet #" << a->get_packet_index() << " in '" << a->get_file_path()
               << "': Integer second timestamp is NONE";
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
    // Catch signals that aren't programming errors
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // Open output file for writing
    // Note that stream is closed implicitly at destruction
    file_path_out = args.file_path_out;
    file_out.open(args.file_path_out, std::ios::out | std::ios::binary | std::ios::trunc);
    if (file_out.fail()) {
        std::stringstream ss;
        ss << "Failed to open output file '" << args.file_path_out << "'";
        throw std::runtime_error(ss.str());
    }

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

    progresscpp::ProgressBar progress(total_file_size_bytes, 70);

    try {
        // Loop until there are no more packets left in any input file
        while (!input_stream_queue.empty()) {
            // Get earliest input packet from queue
            InputStreamPtr stream{input_stream_queue.top()};
            input_stream_queue.pop();

            // Write input packet to output
            stream->write(file_out);

            // Read next packet and insert at the right place into queue if any left
            if (stream->read_next_packet()) {
                input_stream_queue.push(stream);
            }

            // Handle progress bar
            progress += sizeof(uint32_t) * stream->get_header().packet_size;
            if (progress.get_ticks() % 65536 == 0) {
                progress.display();
            }
        }

        progress.done();
    } catch (...) {
        // Cleanup and rethrow
        remove_output_file();
        throw;
    }
}

}  // namespace vrt::merge
