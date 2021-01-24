#include "process.h"

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>

#include "vrt/vrt_types.h"

#include "Progress-CPP/ProgressBar.hpp"
#include "common/input_stream.h"
#include "common/output_stream.h"
#include "program_arguments.h"

namespace vrt::truncate {

// For convenience
using PacketPtr = std::shared_ptr<vrt_packet>;

/**
 * Constructor.
 *
 * \param args Program arguments.
 */
Processor::Processor(const ProgramArguments& args) : program_args_{args} {}

/**
 * Calculate begin (inclusive) and end (exclusive) index
 * \return (begin, end)
 */
std::tuple<uint64_t, uint64_t> Processor::calculate_begin_end() const {
    uint64_t b{0};
    uint64_t e{std::numeric_limits<uint64_t>::max()};

    bool set_begin{program_args_.begin != 0};
    bool set_end{program_args_.end != std::numeric_limits<uint64_t>::max()};
    bool set_count{program_args_.count != std::numeric_limits<uint64_t>::max()};

    // Note that there has already been a check if all three are set
    if (set_begin && set_end) {
        b = program_args_.begin;
        e = program_args_.end;
    } else if (set_begin && set_count) {
        b = program_args_.begin;
        e = program_args_.begin + program_args_.count;
    } else if (set_count && set_end) {
        // There has already been a check that count <= end
        b = program_args_.end - program_args_.count;
        e = program_args_.end;
    } else if (set_begin) {
        b = program_args_.begin;
    } else if (set_count) {
        e = program_args_.count;
    } else if (set_end) {
        e = program_args_.end;
    }

    return {b, e};
}

/**
 * Process file contents.
 *
 * \throw std::runtime_error If there's an error.
 */
void Processor::process() {
    common::InputStream  input_stream(program_args_.file_path_in, program_args_.do_byte_swap);
    common::OutputStream output_stream(program_args_.file_path_out);

    // Calculate begin and end
    uint64_t begin;
    uint64_t end;
    std::tie(begin, end) = calculate_begin_end();

    uint64_t written{0};

    // Progress bar
    progresscpp::ProgressBar progress(end, 70);

    // Go over all packets in input file
    uint64_t i{0};
    for (;; ++i) {
        // Always read packet, so we get header size for progress bar
        if (!input_stream.read_next_packet()) {
            break;
        }

        // Stop condition
        if (i >= end) {
            break;
        }

        if (i >= begin) {
            // Write input packet to output
            PacketPtr packet{input_stream.get_packet()};
            output_stream.write(input_stream.get_buffer(), packet->header.packet_size);
            written++;
        }

        // Handle progress bar
        progress += 1;
        if (progress.get_ticks() % 64 == 0) {
            progress.display();
        }
    }

    progress.done();

    // Warn if not all packets were printed
    if (end != std::numeric_limits<uint64_t>::max() && written != end - begin) {
        std::cerr << "Warning: Did not truncate all packets" << std::endl;
    }
}

}  // namespace vrt::truncate
