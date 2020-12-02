#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_words.h>
#include <vrt/vrt_write.h>

/* Buffer size in 32-bit words */
static const size_t SIZE{515};

int main() {
    /* Packet data buffer */
    std::array<uint32_t, SIZE> b;

    /* Initialize to reasonable values */
    vrt_packet p;
    vrt_init_packet(&p);

    /* Configure */
    p.header.packet_type         = VRT_PT_IF_CONTEXT;
    p.fields.stream_id           = 0xDEADBEEF;
    p.if_context.has.bandwidth   = true;
    p.if_context.bandwidth       = 45.0;
    p.if_context.has.sample_rate = true;
    p.if_context.sample_rate     = 236.0;

    /* Write packet */
    int32_t size{vrt_write_packet(&p, b.data(), SIZE, true)};
    if (size < 0) {
        std::cerr << "Failed to write packet: " << vrt_string_error(size) << std::endl;
        return EXIT_FAILURE;
    }

    /* Write generated packet to file */
    std::string   file_path("if_context_100.vrt");
    std::ofstream fs(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!fs) {
        std::cerr << "Failed to open file '" << file_path << "'" << std::endl;
        return EXIT_FAILURE;
    }
    for (int i{0}; i < 100; ++i) {
        fs.write(reinterpret_cast<char*>(b.data()), sizeof(uint32_t) * size);
        if (!fs) {
            std::cerr << "Failed to write to file '" << file_path << "'" << std::endl;
            return EXIT_FAILURE;
        }
    }

    /* Cleanup */
    fs.close();

    return EXIT_SUCCESS;
}
