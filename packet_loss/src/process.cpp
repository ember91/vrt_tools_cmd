#include "process.h"

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

#include "Progress-CPP/ProgressBar.hpp"
#include "input_stream.h"
#include "output_stream.h"
#include "program_arguments.h"

namespace vrt::packet_loss {

namespace fs = ::std::filesystem;

// For convenience
using PacketPtr = std::shared_ptr<vrt_packet>;

/**
 * Constructor.
 *
 * \param args Program arguments.
 */
Processor::Processor(const ProgramArguments& args) : program_args_{args}, e2_{rd_()}, dist_{0.0, 1.0} {}

/**
 * Calculate if a packet shall be lost.
 *
 * \return True if the packet shall be considered lost.
 */
bool Processor::lost() {
    double r{dist_(e2_)};
    double lim{program_args_.prob_packet_loss};
    if (program_args_.prob_burst_loss != 0.0 && prev_lost_) {
        lim = program_args_.prob_burst_loss;
    }

    prev_lost_ = r <= lim;
    return prev_lost_;
}

/**
 * Process file contents.
 *
 * \throw std::runtime_error If there's an error.
 */
void Processor::process() {
    InputStream  input_stream(program_args_.file_path_in, program_args_.do_byte_swap);
    OutputStream output_stream(program_args_.file_path_out);

    // Progress bar
    progresscpp::ProgressBar progress(static_cast<size_t>(input_stream.get_file_size()), 70);

    // Number of lost packets
    uint64_t n_lost{0};

    // Go over all packets in input file
    uint64_t i;
    for (i = 0;; ++i) {
        // Always read packet, so we get header size for progress bar
        if (!input_stream.read_next_packet()) {
            break;
        }
        PacketPtr packet{input_stream.get_packet()};

        if (lost()) {
            n_lost++;
        } else {
            // Write input packet to output
            input_stream.write(output_stream.get_file());
        }

        // Handle progress bar
        progress += sizeof(uint32_t) * packet->header.packet_size;
        if (progress.get_ticks() % 65536 == 0) {
            progress.display();
        }
    }

    progress.done();

    if (i == 0) {
        std::cerr << "Warning: No packets in file" << std::endl;
    } else {
        if (n_lost == 0) {
            std::cerr << "Warning: 0 out of " << i << " packets were lost. Try increasing probability of packet loss."
                      << std::endl;
        } else {
            std::streamsize initial_prec{std::cout.precision()};
            std::cout << std::fixed << std::setprecision(2) << n_lost << " out of " << i << " packets ("
                      << 100.0 * static_cast<double>(n_lost) / static_cast<double>(i) << " %) were lost" << std::endl;

            // Reset streams to default
            std::cout << std::setprecision(initial_prec) << std::defaultfloat;
        }
    }
}

}  // namespace vrt::packet_loss
