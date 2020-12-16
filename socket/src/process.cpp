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

#include <iostream>

#include <vrt/vrt_types.h>

#include "Progress-CPP/ProgressBar.hpp"
#include "common/input_stream.h"
#include "libsocket/headers/exception.hpp"
#include "libsocket/headers/inetclientdgram.hpp"
#include "libsocket/headers/libinetsocket.h"
#include "program_arguments.h"
#include "time_difference.h"

namespace vrt::socket {

// For convenience
using PacketPtr = std::shared_ptr<vrt_packet>;

/**
 * Process file contents.
 *
 * \param args Program arguments.
 *
 * \throw std::runtime_error If there's an error.
 */
void process(const ProgramArguments& args) {
    common::InputStream input_stream(args.file_path_in, args.do_byte_swap);

    // Progress bar
    progresscpp::ProgressBar progress(static_cast<uint64_t>(input_stream.get_file_size()), 70);

    // Time difference calculator
    TimeDifference time_diff(args.sample_rate);

    try {
        libsocket::inet_dgram_client sock(args.host, args.service, LIBSOCKET_IPv4, 0);

        // Go over all packets in input file
        uint64_t i{0};
        for (;; ++i) {
            if (!input_stream.read_next_packet()) {
                break;
            }

            // Find Class ID, Stream ID combination in map, or construct new output ID if needed
            PacketPtr pkt{input_stream.get_packet()};

            // Sleep
            std::this_thread::sleep_for(time_diff.calculate(pkt));

            sock.snd(input_stream.get_buffer().data(), sizeof(uint32_t) * pkt->header.packet_size, 0);

            // Handle progress bar
            progress += sizeof(uint32_t) * pkt->header.packet_size;
            if (progress.get_ticks() % 65536 == 0) {
                progress.display();
            }
        }
    } catch (const libsocket::socket_exception& exc) {
        std::stringstream ss;
        ss << "Socket send error: ";
        ss << exc.mesg;
        throw std::runtime_error(ss.str());
    }

    progress.done();

    std::cout.flush();
}

}  // namespace vrt::socket
