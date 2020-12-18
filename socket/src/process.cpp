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
#include "libsocket/headers/inetclientstream.hpp"
#include "libsocket/headers/libinetsocket.h"
#include "program_arguments.h"
#include "time_difference.h"

namespace vrt::socket {

// For convenience
using PacketPtr = std::shared_ptr<vrt_packet>;
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

    // Time difference calculator
    TimeDifference time_diff;
    time_diff.set_sample_rate(args.sample_rate);

    std::unique_ptr<libsocket::inet_dgram_client> sock_udp;
    std::unique_ptr<libsocket::inet_stream>       sock_tcp;

    try {
        if (args.protocol == protocol_type::UDP) {
            sock_udp = std::make_unique<libsocket::inet_dgram_client>(args.host, args.service, LIBSOCKET_IPv4, 0);
        } else {
            sock_tcp = std::make_unique<libsocket::inet_stream>(args.host, args.service, LIBSOCKET_IPv4, 0);
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
                t_0 = t_now;
            }

            // Find Class ID, Stream ID combination in map, or construct new output ID if needed
            PacketPtr pkt{input_stream.get_packet()};

            // Sleep
            tm::duration<int64_t, std::nano>                  td{time_diff.calculate(pkt)};
            tm::time_point<tm::system_clock, tm::nanoseconds> t_pkt_now{
                tm::time_point<tm::system_clock, tm::nanoseconds>(td)};

            std::this_thread::sleep_for(td - (t_now - t_0));

            if (args.protocol == protocol_type::UDP) {
                sock_udp->snd(input_stream.get_buffer().data(), sizeof(uint32_t) * pkt->header.packet_size, 0);
            } else {
                sock_tcp->snd(input_stream.get_buffer().data(), sizeof(uint32_t) * pkt->header.packet_size, 0);
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