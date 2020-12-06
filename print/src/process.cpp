#include "process.h"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

#include "byte_swap.h"
#include "program_arguments.h"
#include "stringify.h"
#include "type_printer.h"

namespace vrt::print {

/**
 * Parse header.
 *
 * \param buf    Input buffer.
 * \param i      Packet index.
 * \param header Header.
 *
 * \return Number of read words.
 *
 * \throw std::runtime_error If there is an unknown error.
 */
static int32_t parse_header(std::vector<uint32_t>* buf, size_t i, vrt_header* header) {
    int32_t words{vrt_read_header(buf->data(), buf->size(), header, true)};
    if (words < 0) {
        // Try again, but without validation
        int32_t words_no_val{vrt_read_header(buf->data(), buf->size(), header, false)};
        if (words_no_val < 0) {
            // Should never end up here, since buffer size is sufficient
            std::stringstream ss;
            ss << "Packet #" << i << ": Unknown header parse error: " << vrt_string_error(words);
            throw std::runtime_error(ss.str());
        }
        std::cerr << "Warning: Failed to validate header: " << vrt_string_error(words) << '\n';
        words = words_no_val;
    }

    return words;
}

/**
 * Parse fields section.
 *
 * \param buf           Input buffer.
 * \param i             Packet index.
 * \param header        Header.
 * \param words_header  Words occupied by header.
 * \param fields        Fields struct to write into [out].
 *
 * \return Number of read words.
 *
 * \throw std::runtime_error If there is an unknown error.
 */
static int32_t parse_fields(std::vector<uint32_t>* buf,
                            size_t                 i,
                            const vrt_header&      header,
                            int32_t                words_header,
                            vrt_fields*            fields) {
    int32_t words{
        vrt_read_fields(&header, buf->data() + words_header, header.packet_size - words_header, fields, true)};
    if (words < 0) {
        // Try again, but without validation
        int32_t words_no_val{
            vrt_read_fields(&header, buf->data() + words_header, header.packet_size - words_header, fields, false)};
        if (words_no_val < 0) {
            // Should never end up here, since buffer size is sufficient
            std::stringstream ss;
            ss << "Packet #" << i << ": Unknown fields section parse error: " << vrt_string_error(words);
            throw std::runtime_error(ss.str());
        }
        std::cerr << "Warning: Failed to validate fields section: " << vrt_string_error(words) << '\n';
        words = words_no_val;
    }

    return words;
}

/**
 * Parse body.
 *
 * \param i                                 Packet index.
 * \param header                            Header.
 * \param words_header_fields_if_context    Words occupied by header, fields, and IF context in total.
 *
 * \return Number of read words.
 *
 * \throw std::runtime_error If packet size reported by header doesn't match calculated size.
 */
static int32_t parse_body(size_t i, const vrt_header& header, int32_t words_header_fields_if_context) {
    int32_t words_non_body{words_header_fields_if_context + vrt_words_trailer(&header)};
    switch (header.packet_type) {
        case VRT_PT_IF_DATA_WITHOUT_STREAM_ID:
        case VRT_PT_IF_DATA_WITH_STREAM_ID:
        case VRT_PT_EXT_DATA_WITHOUT_STREAM_ID:
        case VRT_PT_EXT_DATA_WITH_STREAM_ID: {
            // Sanity check
            if (words_non_body > header.packet_size) {
                std::stringstream ss;
                ss << "Packet #" << i << ": Packet size " << header.packet_size << " but " << words_non_body
                   << " words required for header, fields section, and trailer";
                throw std::runtime_error(ss.str());
            }
            break;
        }
        case VRT_PT_IF_CONTEXT: {
            // Sanity check. Exact match here.
            if (words_non_body != header.packet_size) {
                std::stringstream ss;
                ss << "Packet #" << i << ": Packet size " << header.packet_size << " but " << words_non_body
                   << " words required for header, fields section, and context";
                throw std::runtime_error(ss.str());
            }
            break;
        }
        case VRT_PT_EXT_CONTEXT: {
            // Sanity check
            if (words_non_body > header.packet_size) {
                std::stringstream ss;
                ss << "Packet #" << i << ": Packet size " << header.packet_size << " but " << words_non_body
                   << " words required for header and fields section";
                throw std::runtime_error(ss.str());
            }
            break;
        }
        default: {
            words_non_body = 0;
            break;
        }
    }

    return header.packet_size - words_non_body;
}

/**
 * Parse trailer.
 *
 * \param buf       Input buffer.
 * \param i         Packet index.
 * \param header    Header.
 * \param trailer   Trailer to write into [out].
 *
 * \return Number of read words.
 *
 * \throw std::runtime_error If there is an unknown error.
 */
static int32_t parse_trailer(std::vector<uint32_t>* buf, size_t i, const vrt_header& header, vrt_trailer* trailer) {
    int32_t words{vrt_read_trailer(buf->data() + header.packet_size - VRT_WORDS_TRAILER, VRT_WORDS_TRAILER, trailer)};
    if (words < 0) {
        std::stringstream ss;
        ss << "Packet #" << i << ": Unknown trailer parse error: " << vrt_string_error(words);
        throw std::runtime_error(ss.str());
    }

    return words;
}

/**
 * Parse IF context.
 *
 * \param buf                   Input buffer.
 * \param i                     Packet index.
 * \param header                Header.
 * \param words_header_fields   Words occupied by header and fields in total.
 * \param if_context            IF context to write into [out].
 *
 * \return Number of read words.
 *
 * \throw std::runtime_error If there is an unknown error.
 */
static int32_t parse_if_context(std::vector<uint32_t>* buf,
                                size_t                 i,
                                const vrt_header&      header,
                                int32_t                words_header_fields,
                                vrt_if_context*        if_context) {
    int32_t words{0};
    if (header.packet_type == VRT_PT_IF_CONTEXT) {
        words = vrt_read_if_context(buf->data() + words_header_fields, header.packet_size - (words_header_fields),
                                    if_context, true);
        if (words < 0) {
            // Try again, but without validation
            int32_t words_no_val{vrt_read_if_context(buf->data() + words_header_fields,
                                                     header.packet_size - (words_header_fields), if_context, false)};
            if (words_no_val < 0) {
                // Should never end up here, since buffer size is sufficient
                std::stringstream ss;
                ss << "Packet #" << i << ": Unknown IF context parse error: " << vrt_string_error(words);
                throw std::runtime_error(ss.str());
            }
            std::cerr << "Warning: Failed to validate IF context: " << vrt_string_error(words) << '\n';
            words = words_no_val;
        }
    }

    return words;
}

/**
 * Process file contents.
 *
 * \param args Program arguments.
 *
 * \throw std::runtime_error If there's an error.
 */
void process(const ProgramArguments& args) {
    // Preallocate so it has room for header
    std::vector<uint32_t> buf(VRT_WORDS_HEADER);

    std::ifstream file;
    file.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);

    try {
        file.open(args.file_path, std::ios::in | std::ios::binary);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to open file '" << args.file_path << "'";
        throw std::runtime_error(ss.str());
    }

    // Number of printed packets
    size_t n_printed_packets{0};

    // Current packet index
    size_t i;

    // Note that we must go through all packets, since we don't know the size of a packet in the middle of the stream
    // is.
    for (i = 0; n_printed_packets < args.packet_count; ++i) {
        // No need to increase buffer size here, since buf preallocated VRT_SIZE_HEADER words and never shrinks
        try {
            file.read(reinterpret_cast<char*>(buf.data()), sizeof(uint32_t) * VRT_WORDS_HEADER);
        } catch (const std::ios::failure&) {
            if (file.eof()) {
                break;
            }
            std::stringstream ss;
            ss << "Packet #" << i << ": Failed to read header";
            throw std::runtime_error(ss.str());
        }

        // Byte swap if necessary
        if (args.do_byte_swap) {
            buf[0] = bswap_32(buf[0]);
        }

        // Check this after checking EOF
        bool do_print_packet{i >= args.packet_skip};
        if (do_print_packet) {
            n_printed_packets++;
            std::cout << std::string(80, '-') << '\n';
            WriteCols("#", std::to_string(i));
        }

        // Parse, print, and validate header
        vrt_header header;
        int32_t    words_header{parse_header(&buf, i, &header)};
        if (do_print_packet) {
            print_header(header);
        }
        if (header.packet_size == 0) {
            std::stringstream ss;
            ss << "Packet #" << i << ": Header has packet_size 0 words";
            throw std::runtime_error(ss.str());
        }

        // No need to actually read packet. Just skip ahead to the next.
        if (!do_print_packet) {
            try {
                file.seekg(sizeof(uint32_t) * (header.packet_size - words_header), std::ios_base::cur);
            } catch (const std::ios::failure&) {
                throw std::runtime_error("Failed to seek in file");
            }
            continue;
        }

        // Enlarge read buffer for the next section if needed
        if (buf.size() < header.packet_size) {
            buf.resize(header.packet_size);
        }

        try {
            file.read(reinterpret_cast<char*>(buf.data() + words_header),
                      sizeof(uint32_t) * (header.packet_size - words_header));
        } catch (const std::ios::failure&) {
            std::stringstream ss;
            ss << "Packet #" << i << ": Failed to read remainder of packet";
            throw std::runtime_error(ss.str());
        }
        if (args.do_byte_swap) {
            for (int j{1}; j < header.packet_size; ++j) {
                buf[j] = bswap_32(buf[j]);
            }
        }

        // Parse, print, and validate fields
        vrt_fields fields;
        int32_t    words_fields{parse_fields(&buf, i, header, words_header, &fields)};
        print_fields(header, fields);

        // Parse, print, and validate IF context
        int32_t        words_if_context{0};
        vrt_if_context if_context;
        if (header.packet_type == VRT_PT_IF_CONTEXT) {
            words_if_context = parse_if_context(&buf, i, header, words_header + words_fields, &if_context);
        }

        // Parse, print, and validate body
        int32_t words_body{parse_body(i, header, words_header + words_fields + words_if_context)};
        print_body(header, words_body);

        if (header.packet_type == VRT_PT_IF_CONTEXT) {
            print_if_context(if_context);
        }

        // Read trailer if available. This may backfire sometimes when a context type packet has the trailer bit set,
        // but that's nonstandard anyway.
        if (header.has.trailer) {
            vrt_trailer trailer;
            parse_trailer(&buf, i, header, &trailer);
            print_trailer(trailer);
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
