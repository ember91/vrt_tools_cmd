#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_write.h>

#include "generate_tone.h"

/* Size of packet in 32-bit words */
static const size_t SIZE{515};

int main() {
    /* Packet data buffer */
    std::array<uint32_t, SIZE> b;

    /* Generate signal data */
    std::vector<float> s{vrt::generate_tone(SIZE - 3)};

    /* Initialize to reasonable values */
    vrt_packet p;
    vrt_init_packet(&p);

    /* Configure */
    p.header.packet_type         = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p.header.has.trailer         = true;
    p.header.packet_size         = SIZE;
    p.fields.stream_id           = 0xDEADBEEF;
    p.trailer.has.reference_lock = true;
    p.trailer.reference_lock     = true;
    p.words_body                 = s.size();
    p.body                       = s.data();

    /* Write packet */
    int32_t size{vrt_write_packet(&p, b.data(), SIZE, true)};
    if (size < 0) {
        std::cerr << "Failed to write packet: " << vrt_string_error(size) << std::endl;
        return EXIT_FAILURE;
    }

    /* Write generated packet to file */
    std::string   file_path("data_100.vrt");
    std::ofstream file;
    file.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
    try {
        file.open(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
    } catch (const std::ios::failure&) {
        std::cerr << "Failed to open file '" << file_path << "'" << std::endl;
        return EXIT_FAILURE;
    }
    try {
        for (int i{0}; i < 100; ++i) {
            file.write(reinterpret_cast<char*>(b.data()), sizeof(uint32_t) * SIZE);
        }
    } catch (const std::ios::failure&) {
        std::cerr << "Failed to write to file '" << file_path << "'" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
