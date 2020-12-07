#include "input_stream.h"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

#include "byte_swap.h"

namespace vrt::length {

/**
 * Constructor.
 *
 * \param file_path    Path to file.
 * \param do_byte_swap True if byte swap before parsing.
 *
 * \throw std::runtime_error On read or parse error.
 */
InputStream::InputStream(const std::filesystem::path& file_path, bool do_byte_swap) : do_byte_swap_{do_byte_swap} {
    file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);

    // Start at end so file size is available
    try {
        file_.open(file_path, std::ios::in | std::ios::binary | std::ios::ate);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to open file '" << file_path << "'";
        throw std::runtime_error(ss.str());
    }

    // Get file size
    try {
        file_size_bytes_ = file_.tellg();
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to get file size of file '" << file_path << "'";
        throw std::runtime_error(ss.str());
    }

    // Go to start
    try {
        file_.seekg(0);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to seek in file '" << file_path << "'";
        throw std::runtime_error(ss.str());
    }

    // Preallocate so it has room for header
    buf_.resize(VRT_WORDS_HEADER);
}

/**
 * Read next packet in stream.
 *
 * \return False if End Of File.
 *
 * \throw std::runtime_error On read or parse error.
 */
bool InputStream::read_next() {
    // No need to increase buffer size here, since buf preallocated VRT_SIZE_HEADER words and never shrinks
    try {
        file_.read(reinterpret_cast<char*>(buf_.data()), sizeof(uint32_t) * VRT_WORDS_HEADER);
    } catch (const std::ios::failure&) {
        if (file_.eof()) {
            return false;
        }
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << ": Failed to read header";
        throw std::runtime_error(ss.str());
    }

    // Byte swap header section if necessary
    if (do_byte_swap_) {
        buf_[0] = bswap_32(buf_[0]);
    }

    // Parse header
    packet_ = std::make_shared<vrt_packet>();
    int32_t words_header{vrt_read_header(buf_.data(), buf_.size(), &packet_->header, true)};
    if (words_header < 0) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << ": Failed to parse header: " << vrt_string_error(words_header);
        throw std::runtime_error(ss.str());
    }

    // Enlarge read buffer for the next section if needed
    if (buf_.size() < packet_->header.packet_size) {
        buf_.resize(packet_->header.packet_size);
    }

    // Read remainder
    try {
        file_.read(reinterpret_cast<char*>(buf_.data() + words_header),
                   sizeof(uint32_t) * (packet_->header.packet_size - words_header));
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << ": Failed to read remainder of packet";
        throw std::runtime_error(ss.str());
    }

    // Byte swap fields section if necessary
    int32_t words_fields = vrt_words_fields(&packet_->header);
    if (do_byte_swap_) {
        for (size_t j{1}; j < static_cast<size_t>(packet_->header.packet_size); ++j) {
            buf_[j] = bswap_32(buf_[j]);
        }
    }

    // Parse fields
    words_fields = vrt_read_fields(&packet_->header, buf_.data() + words_header, buf_.size() - words_header,
                                   &packet_->fields, true);
    if (words_fields < 0) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << ": Failed to parse fields section: " << vrt_string_error(words_fields);
        throw std::runtime_error(ss.str());
    }

    // Parse IF context, if any
    if (packet_->header.packet_type == VRT_PT_IF_CONTEXT) {
        int32_t words_header_fields{words_header + words_fields};
        vrt_read_if_context(buf_.data() + words_header_fields, buf_.size() - words_header_fields, &packet_->if_context,
                            true);
    }

    // Skip trailer

    pkt_idx_++;

    return true;
}

}  // namespace vrt::length
