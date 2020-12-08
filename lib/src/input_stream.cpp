#include "input_stream.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <vrt/vrt_init.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

#include "byte_swap.h"

namespace fs = std::filesystem;

namespace vrt {

/**
 * Constructor. Open input file for reading.
 *
 * \param file_path    Path to file.
 * \param do_byte_swap True if byte swap before parsing.
 *
 * \throw std::runtime_error On read or parse error.
 */
InputStream::InputStream(fs::path file_path, bool do_byte_swap)
    : file_path_{std::move(file_path)}, do_byte_swap_{do_byte_swap} {
    file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);

    // Open file for reading at end so file size is available
    try {
        file_.open(file_path_, std::ios::in | std::ios::binary | std::ios::ate);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to open input file '" << file_path_ << "'";
        throw std::runtime_error(ss.str());
    }

    // Get file size
    try {
        file_size_bytes_ = file_.tellg();
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to get size of file '" << file_path_ << "'";
        throw std::runtime_error(ss.str());
    }

    // Go to start
    try {
        file_.seekg(0);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to seek in file '" << file_path_ << "'";
        throw std::runtime_error(ss.str());
    }

    // Preallocate room for header at least
    buf_.resize(VRT_WORDS_HEADER);
    buf_byte_swap_.resize(VRT_WORDS_HEADER);
}

/**
 * Read next packet in stream.
 *
 * \return False if End Of File.
 *
 * \throw std::runtime_error On read or parse error.
 */
bool InputStream::read_next_packet() {
    // No need to increase buffer size here, since buf preallocated VRT_SIZE_HEADER words and never shrinks
    try {
        file_.read(reinterpret_cast<char*>(buf_.data()), sizeof(uint32_t) * VRT_WORDS_HEADER);
    } catch (const std::ios::failure&) {
        if (file_.eof()) {
            return false;
        }
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << " in '" << file_path_ << "': Failed to read header";
        throw std::runtime_error(ss.str());
    }

    // Byte swap header if necessary
    if (do_byte_swap_) {
        buf_byte_swap_[0] = bswap_32(buf_[0]);
    } else {
        buf_byte_swap_[0] = buf_[0];
    }

    // Parse and validate header
    packet_ = std::make_shared<vrt_packet>();
    int32_t words_header{vrt_read_header(buf_byte_swap_.data(), buf_byte_swap_.size(), &packet_->header, true)};
    if (words_header < 0) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << " in '" << file_path_
           << "': Failed to parse header: " << vrt_string_error(words_header);
        throw std::runtime_error(ss.str());
    }

    // Enlarge read buffer for the next section if needed
    if (buf_.size() < packet_->header.packet_size) {
        buf_.resize(packet_->header.packet_size);
        buf_byte_swap_.resize(packet_->header.packet_size);
    }

    // Read remainder
    try {
        file_.read(reinterpret_cast<char*>(buf_.data() + words_header),
                   sizeof(uint32_t) * (packet_->header.packet_size - words_header));
    } catch (const std::ios::failure&) {
        if (file_.eof()) {
            // Just a warning. Mark as EOF.
            std::cerr << "Warning: End of file in middle of packet #" << pkt_idx_ << '\n';
            return false;
        }
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << " in '" << file_path_ << "': Failed to read remainder of packet";
        throw std::runtime_error(ss.str());
    }

    // Byte swap remainder
    if (do_byte_swap_) {
        for (size_t j{1}; j < static_cast<size_t>(packet_->header.packet_size); ++j) {
            buf_byte_swap_[j] = bswap_32(buf_[j]);
        }
    } else {
        std::copy_n(buf_.data() + 1, packet_->header.packet_size - 1, buf_byte_swap_.data() + 1);
    }

    // Parse and validate fields section
    int32_t words_fields{vrt_read_fields(&packet_->header, buf_byte_swap_.data() + words_header,
                                         buf_byte_swap_.size() - words_header, &packet_->fields, true)};
    if (words_fields < 0) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << " in '" << file_path_
           << "': Failed to parse fields section: " << vrt_string_error(words_fields);
        throw std::runtime_error(ss.str());
    }

    // Parse IF context, if any
    if (packet_->header.packet_type == VRT_PT_IF_CONTEXT) {
        int32_t words_header_fields{words_header + words_fields};
        vrt_read_if_context(buf_byte_swap_.data() + words_header_fields, buf_byte_swap_.size() - words_header_fields,
                            &packet_->if_context, true);
    }

    // Parse trailer, if any
    if (packet_->header.has.trailer) {
        vrt_read_trailer(buf_byte_swap_.data() + packet_->header.packet_size - 1,
                         buf_byte_swap_.size() - (packet_->header.packet_size - 1), &packet_->trailer);
    }

    pkt_idx_++;

    return true;
}

/**
 * Write buffer to output file.
 *
 * \param of Output file.
 *
 * \throw std::runtime_error On I/O error.
 */
void InputStream::write(std::ofstream& of) {
    try {
        of.write(reinterpret_cast<char*>(buf_.data()), sizeof(uint32_t) * packet_->header.packet_size);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to write to output file";
        throw std::runtime_error(ss.str());
    }
}

}  // namespace vrt
