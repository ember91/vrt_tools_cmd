#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_write.h>

#include "generate_packet_sequence.h"
#include "generate_tone.h"

/* Size of packet in 32-bit words */
static const size_t SIZE{515};

int main() {
    /* Generate signal data */
    std::vector<float> s{vrt::generate_tone(SIZE - 10)};

    /* Initialize to reasonable values */
    vrt_packet p;
    vrt_init_packet(&p);

    /* Configure */
    p.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p.header.packet_size = SIZE;
    p.words_body         = s.size();
    p.body               = s.data();
    p.header.tsi         = VRT_TSI_GPS;
    p.header.tsf         = VRT_TSF_REAL_TIME;

    const uint64_t                          ps_in_s{1000000000000};
    std::random_device                      rd;
    std::mt19937                            gen(rd());
    std::uniform_int_distribution<uint64_t> distrib(1, ps_in_s / 10);

    const size_t N_PACKETS{100};

    try {
        auto func = [&](int i, vrt_packet* p) {
            p->fields.fractional_seconds_timestamp += distrib(gen);
            if (p->fields.fractional_seconds_timestamp >= ps_in_s) {
                uint64_t n{p->fields.fractional_seconds_timestamp / ps_in_s};
                p->fields.integer_seconds_timestamp += n;
                p->fields.fractional_seconds_timestamp -= n * ps_in_s;
            }
        };
        p.fields.stream_id = 0;
        vrt::generate_packet_sequence("time_0.vrt", &p, N_PACKETS, func);
        p.fields.stream_id                    = 1;
        p.fields.integer_seconds_timestamp    = 0;
        p.fields.fractional_seconds_timestamp = 0;
        vrt::generate_packet_sequence("time_1.vrt", &p, N_PACKETS, func);
        p.fields.stream_id                    = 2;
        p.fields.integer_seconds_timestamp    = 0;
        p.fields.fractional_seconds_timestamp = 0;
        vrt::generate_packet_sequence("time_2.vrt", &p, N_PACKETS, func);
        p.fields.stream_id                    = 3;
        p.fields.integer_seconds_timestamp    = 0;
        p.fields.fractional_seconds_timestamp = 0;
        vrt::generate_packet_sequence("time_3.vrt", &p, N_PACKETS, func);
    } catch (const std::runtime_error& exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
