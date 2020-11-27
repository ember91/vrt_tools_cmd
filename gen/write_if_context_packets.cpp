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
    vrt_header     h;
    vrt_fields     f;
    vrt_if_context c;
    vrt_init_header(&h);
    vrt_init_fields(&f);
    vrt_init_if_context(&c);

    /* Configure */
    h.packet_type     = VRT_PT_IF_CONTEXT;
    f.stream_id       = 0xDEADBEEF;
    c.has.bandwidth   = true;
    c.bandwidth       = 45.0;
    c.has.sample_rate = true;
    c.sample_rate     = 236.0;

    h.packet_size = VRT_WORDS_HEADER + vrt_words_fields(&h) + vrt_words_if_context(&c);

    /* Write header */
    int32_t offset{0};
    int32_t rv{vrt_write_header(&h, b.data() + offset, SIZE - offset, true)};
    if (rv < 0) {
        std::cerr << "Failed to write header: " << vrt_string_error(rv) << std::endl;
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write fields */
    rv = vrt_write_fields(&h, &f, b.data() + offset, SIZE - offset, true);
    if (rv < 0) {
        std::cerr << "Failed to write fields section: " << vrt_string_error(rv) << std::endl;
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write context */
    rv = vrt_write_if_context(&c, b.data() + offset, SIZE - offset, true);
    if (rv < 0) {
        std::cerr << "Failed to write context: " << vrt_string_error(rv) << std::endl;
        return EXIT_FAILURE;
    }
    offset += rv;
    int32_t size{offset};

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

    fs.close();

    return EXIT_SUCCESS;
}
