#include "input_stream.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>

#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_words.h>

#include "byte_swap.h"

namespace vrt::split {

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
        std::runtime_error(ss.str());
    }

    // Go to start
    try {
        file_.seekg(0);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to seek in file '" << file_path << "'";
        std::runtime_error(ss.str());
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
    std::array<uint32_t, VRT_WORDS_HEADER + VRT_WORDS_MAX_FIELDS> buf_header_fields{};

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
        buf_header_fields[0] = bswap_32(buf_[0]);
    } else {
        buf_header_fields[0] = buf_[0];
    }

    // Parse header
    packet_ = std::make_shared<vrt_packet>();
    int32_t words_header{vrt_read_header(buf_header_fields.data(), buf_header_fields.size(), &packet_->header, true)};
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
        for (int j{1}; j < words_fields + 1; ++j) {
            buf_header_fields[j] = bswap_32(buf_[j]);
        }
    } else {
        std::copy_n(buf_.data() + 1, words_fields, buf_header_fields.data() + 1);
    }

    // Parse fields
    words_fields = vrt_read_fields(&packet_->header, buf_header_fields.data() + words_header,
                                   buf_header_fields.size() - words_header, &packet_->fields, true);
    if (words_fields < 0) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << ": Failed to parse fields section: " << vrt_string_error(words_fields);
        throw std::runtime_error(ss.str());
    }

    pkt_idx_++;

    return true;
}

}  // namespace vrt::split
