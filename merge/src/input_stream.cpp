#include "input_stream.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include <vrt/vrt_init.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_words.h>

#include "byte_swap.h"

namespace fs = std::filesystem;

namespace vrt::merge {

/**
 * Constructor. Open input file for reading.
 *
 * \param file_path    File path to input file.
 * \param do_byte_swap True if byte swap shall be used.
 *
 * \throw std::runtime_error If file failed to open.
 */
InputStream::InputStream(fs::path file_path, bool do_byte_swap)
    : file_path_{std::move(file_path)}, do_byte_swap_{do_byte_swap} {
    // Zero initialize
    vrt_init_header(&header_);
    vrt_init_fields(&fields_);

    // Open file for reading
    // Start at end so file size is available
    file_in_.open(file_path_, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file_in_) {
        // Note that destructor is not run if this fails
        std::stringstream ss;
        ss << "Failed to open input file '" << file_path_ << "'";
        std::runtime_error(ss.str());
    }

    // Get file size
    file_size_bytes_ = file_in_.tellg();
    if (!file_in_) {
        // Note that destructor is not run if this fails
        std::stringstream ss;
        ss << "Failed to get file size of file '" << file_path_ << "'";
        std::runtime_error(ss.str());
    }

    // Go to start
    file_in_.seekg(0);
    if (!file_in_) {
        // Note that destructor is not run if this fails
        std::stringstream ss;
        ss << "Failed to seek in file '" << file_path_ << "'";
        std::runtime_error(ss.str());
    }

    // Make space for header at least
    buf_.resize(VRT_WORDS_HEADER);
}

/**
 * Read next packet in stream, if any, into internal buffer.
 *
 * \return True if there was any next packet.
 *
 * \throw std::runtime_error On I/O error.
 */
bool InputStream::read_next_packet() {
    // Preallocate room for, perhaps byte swapped, header and fields
    std::array<uint32_t, VRT_WORDS_HEADER + VRT_WORDS_MAX_FIELDS> buf_header_fields{};

    // Read header
    // We know here that buffer size at least has room for the header
    file_in_.read(reinterpret_cast<char*>(buf_.data()), sizeof(uint32_t) * VRT_WORDS_HEADER);
    if (file_in_.gcount() != 0 && file_in_.gcount() != sizeof(uint32_t) * VRT_WORDS_HEADER) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << " in '" << file_path_ << "': Failed to read header";
        throw std::runtime_error(ss.str());
    }
    if (file_in_.eof()) {
        return false;
    }

    // Byte swap if necessary
    if (do_byte_swap_) {
        buf_header_fields[0] = bswap_32(buf_[0]);
    } else {
        buf_header_fields[0] = buf_[0];
    }

    // Parse and validate header
    int32_t words_header{vrt_read_header(buf_header_fields.data(), buf_header_fields.size(), &header_, true)};
    if (words_header < 0) {
        std::stringstream ss;
        ss << "Error when validating packet #" << pkt_idx_ << " header in '" << file_path_
           << "': " << vrt_string_error(words_header);
        throw std::runtime_error(ss.str());
    }

    // Resize buffer if needed
    if (buf_.size() < header_.packet_size) {
        buf_.resize(header_.packet_size);
    }

    // Read packet remainder
    file_in_.read(reinterpret_cast<char*>(buf_.data() + words_header),
                  sizeof(uint32_t) * (header_.packet_size - words_header));
    if (file_in_.gcount() != 0 && file_in_.gcount() != sizeof(uint32_t) * (header_.packet_size - words_header)) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << " in '" << file_path_ << "': Failed to read remainder of packet";
        throw std::runtime_error(ss.str());
    }
    if (file_in_.eof()) {
        // Just a warning. Mark as EOF.
        std::cerr << "Warning: End of file in middle of packet #" << pkt_idx_ << '\n';
        return false;
    }

    // Byte swap fields section if necessary
    int32_t words_fields = vrt_words_fields(&header_);
    if (do_byte_swap_) {
        for (int j{1}; j < words_fields + 1; ++j) {
            buf_header_fields[j] = bswap_32(buf_[j]);
        }
    } else {
        std::copy_n(buf_.data() + 1, words_fields, buf_header_fields.data() + 1);
    }

    // Parse and validate fields section
    words_fields = vrt_read_fields(&header_, buf_header_fields.data() + words_header,
                                   buf_header_fields.size() - words_header, &fields_, true);
    if (words_fields < 0) {
        std::stringstream ss;
        ss << "Error when validating packet #" << pkt_idx_ << " fields section in '" << file_path_
           << "': " << vrt_string_error(words_fields);
        throw std::runtime_error(ss.str());
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
    of.write(reinterpret_cast<char*>(buf_.data()), sizeof(uint32_t) * buf_.size());
    if (!of) {
        std::stringstream ss;
        ss << "Failed to write to output file";
        throw std::runtime_error(ss.str());
    }
}

}  // namespace vrt::merge
