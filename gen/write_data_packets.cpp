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

    /* Cleanup */
    fs.close();

    return EXIT_SUCCESS;
}
