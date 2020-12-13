#include "common/generate_packet_sequence.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>
#include <vrt/vrt_write.h>

#include "common/byte_swap.h"

namespace vrt::common {

namespace fs = ::std::filesystem;

/**
 * Generate packet sequence and write to file.
 *
 * \param file_path Path to output file.
 * \param p         Preconfigured packet to write and change every step.
 * \param n         Number of packets to generate.
 * \param change    Function to run before each write, which may change the written packet.
 */
void generate_packet_sequence(const fs::path&                        file_path,
                              vrt_packet*                            p,
                              uint64_t                               n,
                              const std::function<void(uint64_t i)>& change,
                              bool                                   do_byte_swap) {
    std::ofstream file;
    file.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);

    // Open file
    try {
        file.open(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to open file " << file_path;
        throw std::runtime_error(ss.str());
    }

    // Create data buffer
    std::vector<uint32_t> b;

    for (uint64_t i{0}; i < n; ++i) {
        /* Change fields depending on index */
        change(i);

        // Write packet to buffer
        int32_t size{vrt_write_packet(p, b.data(), b.size(), true)};
        if (size == VRT_ERR_BUFFER_SIZE) {
            b.resize(vrt_words_packet(p));
            size = vrt_write_packet(p, b.data(), b.size(), true);
        }
        if (size < 0) {
            std::stringstream ss;
            ss << "Failed to write VRT packet to buffer: " << vrt_string_error(size);
            throw std::runtime_error(ss.str());
        }

        // Byte swap if enabled
        if (do_byte_swap) {
            for (uint16_t j{0}; j < size; ++j) {
                // Don't care about how to byte swap data...
                b[j] = bswap_32(b[j]);
            }
        }

        // Write buffer to file
        try {
            file.write(reinterpret_cast<char*>(b.data()), sizeof(uint32_t) * size);
        } catch (const std::ios::failure&) {
            std::stringstream ss;
            ss << "Failed to write VRT packet to file " << file_path;
            throw std::runtime_error(ss.str());
        }
    }
}

}  // namespace vrt::common
