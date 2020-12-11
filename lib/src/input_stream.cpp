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

#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

#include "byte_swap.h"

namespace vrt {

namespace fs = ::std::filesystem;

/**
 * Constructor. Open input file for reading.
 *
 * \param file_path    Path to file.
 * \param do_byte_swap True if byte swap before parsing.
 * \param do_validate  True if packets shall be validated.
 *
 * \throw std::runtime_error On read or parse error.
 */
InputStream::InputStream(fs::path file_path, bool do_byte_swap, bool do_validate)
    : file_path_{std::move(file_path)}, do_byte_swap_{do_byte_swap}, do_validate_{do_validate} {
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
    if (!read_parse_header()) {
        return false;
    }

    // Enlarge read buffer for the next section if needed
    if (buf_.size() < packet_->header.packet_size) {
        buf_.resize(packet_->header.packet_size);
        buf_byte_swap_.resize(packet_->header.packet_size);
    }

    // Read remainder
    try {
        file_.read(reinterpret_cast<char*>(buf_.data() + VRT_WORDS_HEADER),
                   sizeof(uint32_t) * (packet_->header.packet_size - VRT_WORDS_HEADER));
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
    int32_t words_fields{vrt_read_fields(&packet_->header, buf_byte_swap_.data() + VRT_WORDS_HEADER,
                                         buf_byte_swap_.size() - VRT_WORDS_HEADER, &packet_->fields, true)};
    if (words_fields < 0) {
        if (do_validate_) {
            std::stringstream ss;
            ss << "Packet #" << pkt_idx_ << " in '" << file_path_
               << "': Failed to parse fields section: " << vrt_string_error(words_fields);
            throw std::runtime_error(ss.str());
        } else {
            // Never any error here, since buffer size is sufficient
            vrt_read_fields(&packet_->header, buf_byte_swap_.data() + VRT_WORDS_HEADER,
                            buf_byte_swap_.size() - VRT_WORDS_HEADER, &packet_->fields, false);
            std::cerr << "Warning: Packet #" << pkt_idx_ << " in '" << file_path_
                      << "': Failed to validate fields section: " << vrt_string_error(words_fields);
        }
    }

    // Parse IF context, if any
    int32_t words_if_context{0};
    if (packet_->header.packet_type == VRT_PT_IF_CONTEXT) {
        int32_t words_header_fields{VRT_WORDS_HEADER + words_fields};
        words_if_context = vrt_read_if_context(buf_byte_swap_.data() + words_header_fields,
                                               buf_byte_swap_.size() - words_header_fields, &packet_->if_context, true);
        if (words_if_context < 0) {
            if (do_validate_) {
                std::stringstream ss;
                ss << "Packet #" << pkt_idx_ << " in '" << file_path_
                   << "': Failed to parse IF context: " << vrt_string_error(words_if_context);
                throw std::runtime_error(ss.str());
            } else {
                // Never any error here, since buffer size is sufficient
                vrt_read_if_context(buf_byte_swap_.data() + words_header_fields,
                                    buf_byte_swap_.size() - words_header_fields, &packet_->if_context, false);
                std::cerr << "Warning: Packet #" << pkt_idx_ << " in '" << file_path_
                          << "': Failed to validate IF context: " << vrt_string_error(words_if_context);
            }
        }
    }

    // Parse trailer, if any
    int32_t words_trailer{0};
    if (packet_->header.has.trailer) {
        words_trailer = vrt_read_trailer(buf_byte_swap_.data() + packet_->header.packet_size - 1,
                                         buf_byte_swap_.size() - (packet_->header.packet_size - 1), &packet_->trailer);
        if (words_trailer < 0) {
            if (do_validate_) {
                std::stringstream ss;
                ss << "Packet #" << pkt_idx_ << " in '" << file_path_
                   << "': Failed to parse trailer: " << vrt_string_error(words_trailer);
                throw std::runtime_error(ss.str());
            } else {
                // Never any error here, since buffer size is sufficient
                vrt_read_trailer(buf_byte_swap_.data() + packet_->header.packet_size - 1,
                                 buf_byte_swap_.size() - (packet_->header.packet_size - 1), &packet_->trailer);
                std::cerr << "Warning: Packet #" << pkt_idx_ << " in '" << file_path_
                          << "': Failed to validate trailer: " << vrt_string_error(words_trailer);
            }
        }
    }

    packet_->words_body =
        packet_->header.packet_size - (VRT_WORDS_HEADER + words_fields + words_if_context + words_trailer);
    if (packet_->words_body < 0) {
        if (do_validate_) {
            std::stringstream ss;
            ss << "Packet #" << pkt_idx_ << " in '" << file_path_ << "': Body is a negative size";
            throw std::runtime_error(ss.str());
        } else {
            packet_->words_body = 0;
            packet_->body       = nullptr;
            std::cerr << "Warning: Packet #" << pkt_idx_ << " in '" << file_path_ << "': Body is a negative size";
        }
    } else {
        packet_->body = buf_.data() + VRT_WORDS_HEADER + words_fields;
    }

    pkt_idx_++;

    return true;
}

/**
 * Skip next packet in stream. More efficient than reading it.
 *
 * \return False if End Of File.
 *
 * \throw std::runtime_error On read or parse error.
 */
bool InputStream::skip_next_packet() {
    if (!read_parse_header()) {
        return false;
    }

    // Jump forward
    try {
        file_.seekg(sizeof(uint32_t) * (packet_->header.packet_size - VRT_WORDS_HEADER), std::ios_base::cur);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to seek in file '" << file_path_ << "'";
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
    try {
        of.write(reinterpret_cast<char*>(buf_.data()), sizeof(uint32_t) * packet_->header.packet_size);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to write to output file";
        throw std::runtime_error(ss.str());
    }
}

/**
 * Skip next packet in stream. More efficient than reading it.
 *
 * \return False if End Of File.
 *
 * \throw std::runtime_error On read or parse error.
 */
bool InputStream::read_parse_header() {
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
    int32_t words_header{vrt_read_header(buf_byte_swap_.data(), buf_byte_swap_.size(), &packet_->header, do_validate_)};
    if (words_header < 0) {
        if (do_validate_) {
            std::stringstream ss;
            ss << "Packet #" << pkt_idx_ << " in '" << file_path_
               << "': Failed to parse header: " << vrt_string_error(words_header);
            throw std::runtime_error(ss.str());
        } else {
            // Never any error here, since buffer size is sufficient
            vrt_read_header(buf_byte_swap_.data(), buf_byte_swap_.size(), &packet_->header, false);
            std::cerr << "Warning: Packet #" << pkt_idx_ << " in '" << file_path_
                      << "': Failed to validate header: " << vrt_string_error(words_header);
        }
    }

    // No infinite loops thank you
    if (packet_->header.packet_size == 0) {
        std::stringstream ss;
        ss << "Packet #" << pkt_idx_ << ": Header has packet_size 0 words";
        throw std::runtime_error(ss.str());
    }

    return true;
}

}  // namespace vrt
