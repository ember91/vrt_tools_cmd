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

#include "vrt/vrt_time.h"
#include "vrt/vrt_types.h"

#include "Progress-CPP/ProgressBar.hpp"
#include "common/input_stream.h"
#include "program_arguments.h"
#include "socket_abstraction.h"

namespace vrt::socket {

// For convenience
using PacketPtr = ::std::shared_ptr<vrt_packet>;
namespace tm    = ::std::chrono;

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

    double sample_rate{args.sample_rate};

    PacketPtr                            pkt_0;
    std::vector<std::unique_ptr<Socket>> sockets;
    sockets.reserve(args.hosts.size());

    try {
        for (const std::string& host : args.hosts) {
            if (args.protocol == protocol_type::UDP) {
                sockets.push_back(std::make_unique<SocketUdp>(host, args.service));
            } else {
                sockets.push_back(std::make_unique<SocketTcp>(host, args.service));
            }
        }

        // Time of first packet
        tm::time_point<tm::system_clock, tm::nanoseconds> t_0;

        // Time of last visual progress bar update
        tm::time_point<tm::system_clock, tm::nanoseconds> t_progress_bar_update;

        // Go over all packets in input file
        uint64_t i{0};
        for (;; ++i) {
            if (!input_stream.read_next_packet()) {
                break;
            }

            // Get current time
            tm::time_point<tm::system_clock, tm::nanoseconds> t_now{tm::system_clock::now()};
            if (i == 0) {
                pkt_0 = input_stream.get_packet();
                t_0   = t_now;
            }

            // Find Class ID, Stream ID combination in map, or construct new output ID if needed
            PacketPtr pkt{input_stream.get_packet()};

            // Calculate time until next packet
            vrt_time time_diff;
            if (vrt_time_difference(pkt.get(), pkt_0.get(), sample_rate, &time_diff) < 0 || time_diff.s < 0) {
                // Don't sleep if error or negative time
                time_diff.s  = 0;
                time_diff.ps = 0;
            }

            tm::duration<int64_t, std::nano> td{tm::seconds(time_diff.s) + tm::nanoseconds(time_diff.ps / 1000)};
            tm::time_point<tm::system_clock, tm::nanoseconds> t_pkt_now{
                tm::time_point<tm::system_clock, tm::nanoseconds>(td)};

            std::this_thread::sleep_for(td - (t_now - t_0));

            for (auto& socket : sockets) {
                socket->send(input_stream.get_buffer().data(), sizeof(uint32_t) * pkt->header.packet_size);
            }

            // Handle progress bar
            progress += sizeof(uint32_t) * pkt->header.packet_size;
            if (std::chrono::duration_cast<tm::seconds>(t_now - t_progress_bar_update).count() != 0) {
                progress.display();
                t_progress_bar_update = t_now;
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
