#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_write.h>

/* Size of packet in 32-bit words */
static const size_t SIZE{515};
/* Sample rate [Hz] */
static const float SAMPLE_RATE{44100.0F};
/* Center frequency [Hz] */
static const float CENTER_FREQUENCY{10000.0F};
/* M_PI in cmath is nonstandard :( */
static const float PI{3.1415926F};

int main() {
    /* Packet data buffer */
    std::array<uint32_t, SIZE> b;

    /* Generate signal data */
    std::array<float, SIZE - 3> s;
    for (int i{0}; i < s.size(); ++i) {
        s[i] = std::sin(2.0F * PI * CENTER_FREQUENCY * i / SAMPLE_RATE);
    }

    /* Initialize to reasonable values */
    vrt_header  h;
    vrt_fields  f;
    vrt_trailer t;
    vrt_init_header(&h);
    vrt_init_fields(&f);
    vrt_init_trailer(&t);

    /* Configure */
    h.packet_type        = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h.has.trailer        = true;
    h.packet_size        = SIZE;
    f.stream_id          = 0xDEADBEEF;
    t.has.reference_lock = true;
    t.reference_lock     = true;

    /* Write header */
    int32_t offset{0};
    int32_t rv{vrt_write_header(&h, b.data() + offset, SIZE - offset, true)};
    if (rv < 0) {
        std::cerr << "Failed to write header: " << vrt_string_error(rv) << std::endl;
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write fields, which in this case is Stream ID */
    rv = vrt_write_fields(&h, &f, b.data() + offset, SIZE - offset, true);
    if (rv < 0) {
        std::cerr << "Failed to write fields section: " << vrt_string_error(rv) << std::endl;
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Copy signal data from signal to packet buffer.
     * This could also have been written directly into the buffer. */
    std::copy_n(s.data(), (SIZE - 3), b.data() + offset);
    offset += SIZE - 3;

    /* Write trailer */
    rv = vrt_write_trailer(&t, b.data() + offset, SIZE - offset, true);
    if (rv < 0) {
        std::cerr << "Failed to write trailer: " << vrt_string_error(rv) << std::endl;
        return EXIT_FAILURE;
    }

    /* Write generated packet to file */
    std::string   file_path("data_100.vrt");
    std::ofstream fs(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!fs) {
        std::cerr << "Failed to open file '" << file_path << "'" << std::endl;
        return EXIT_FAILURE;
    }
    for (int i{0}; i < 100; ++i) {
        fs.write(reinterpret_cast<char*>(b.data()), sizeof(uint32_t) * SIZE);
        if (!fs) {
            std::cerr << "Failed to write to file '" << file_path << "'" << std::endl;
            return EXIT_FAILURE;
        }
    }

    fs.close();

    return EXIT_SUCCESS;
}
