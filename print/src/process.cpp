#include "process.h"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

#include "byte_swap.h"
#include "input_stream.h"
#include "program_arguments.h"
#include "stringify.h"
#include "type_printer.h"

namespace vrt::print {

namespace fs = ::std::filesystem;

using PacketPtr = std::shared_ptr<vrt_packet>;

/**
 * Process file contents.
 *
 * \param args Program arguments.
 *
 * \throw std::runtime_error If there's an error.
 */
void process(const ProgramArguments& args) {
    InputStream input_stream(args.file_path, args.do_byte_swap, false);

    // Number of printed packets
    size_t n_printed_packets{0};

    // Current packet index
    size_t i;

    // Note that we must go through all packets, since we don't know the size of a packet in the middle of the stream
    // is.
    for (i = 0; n_printed_packets < args.packet_count; ++i) {
        // Check this after checking EOF
        bool do_print_packet{i >= args.packet_skip};
        if (do_print_packet) {
            n_printed_packets++;
            std::cout << std::string(80, '-') << '\n';
            WriteCols("#", std::to_string(i));

            input_stream.read_next_packet();
        } else {
            input_stream.skip_next_packet();
            continue;
        }

        PacketPtr packet{input_stream.get_packet()};

        print_header(packet->header);
        print_fields(packet->header, packet->fields);
        print_body(packet->header, packet->words_body);

        if (packet->header.packet_type == VRT_PT_IF_CONTEXT) {
            print_if_context(packet->if_context);
        }
        if (packet->header.has.trailer) {
            print_trailer(packet->trailer);
        }
    }

    // Print some warnings if not all packets were printed
    if (i != 0) {
        if (n_printed_packets == 0) {
            std::cerr << "Warning: Skipped over all " << i << " packet(s)\n";
        } else if (n_printed_packets < args.packet_count && args.packet_count != static_cast<size_t>(-1)) {
            std::cerr << "Warning: Printed only " << n_printed_packets << " out of " << args.packet_count
                      << " packet(s) due to end of file\n";
        }
    }

    // Ensure text is output
    std::cout << std::flush;
}

}  // namespace vrt::print
