#ifndef VRT_TOOLS_CMD_LIB_GENERATE_PACKET_SEQUENCE_H_
#define VRT_TOOLS_CMD_LIB_GENERATE_PACKET_SEQUENCE_H_

#include <fstream>
#include <functional>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>
#include <vrt/vrt_write.h>

namespace vrt {

/**
 * Generate packet sequence and write to file.
 *
 * \param file_path Path to output file.
 * \param p         Preconfigured packet to write and change every step.
 * \param n         Number of packets to generate.
 * \param change    Function to run before each write, which may change the written packet.
 */
void generate_packet_sequence(
    const std::string&                             file_path,
    vrt_packet*                                    p,
    size_t                                         n,
    const std::function<void(int i, vrt_packet*)>& change = [](int i, vrt_packet*) {}) {
    // Open file
    std::ofstream file(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file) {
        std::stringstream ss;
        ss << "Failed to open file '" << file_path << "'";
        throw std::runtime_error(ss.str());
    }

    // Create data buffer
    std::vector<uint32_t> b;

    for (int i{0}; i < n; ++i) {
        /* Change fields depending on index */
        change(i, p);

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

        // Write buffer to file
        file.write(reinterpret_cast<char*>(b.data()), sizeof(uint32_t) * size);
        if (!file) {
            std::stringstream ss;
            ss << "Failed to write VRT packet to file '" << file_path << "'";
            throw std::runtime_error(ss.str());
        }
    }

    // Cleanup
    file.close();
}

}  // namespace vrt

#endif