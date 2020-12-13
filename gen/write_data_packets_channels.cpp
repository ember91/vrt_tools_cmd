#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>

#include "common/generate_packet_sequence.h"
#include "common/generate_tone.h"

using namespace vrt;

/* Size of packet in 32-bit words */
static const size_t SIZE{515};

int main() {
    /* Packet data buffer */
    std::array<uint32_t, SIZE> b;

    /* Generate signal data */
    std::vector<float> s{common::generate_tone(SIZE - 10)};

    /* Initialize to reasonable values */
    vrt_packet p;
    vrt_init_packet(&p);

    /* Configure */
    p.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p.header.packet_size = SIZE;
    p.fields.stream_id   = 0xDEADBEEF;
    p.words_body         = s.size();
    p.body               = s.data();

    const size_t N_PACKETS{100};

    try {
        common::generate_packet_sequence("var_stream_id.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
            p.header.has.class_id = false;
            p.fields.stream_id    = i % 4;
        });
        common::generate_packet_sequence("var_def_stream_id.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.has.class_id = false;
            if (i % 4 == 0) {
                p.header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            } else {
                p.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
                p.fields.stream_id   = i % 4;
            }
        });
        common::generate_packet_sequence("var_oui.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.has.class_id = true;
            p.header.packet_type  = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p.fields.class_id.oui = i % 4;
        });
        common::generate_packet_sequence("var_icc.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.has.class_id                    = true;
            p.header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p.fields.class_id.information_class_code = i % 4;
        });
        common::generate_packet_sequence("var_pcc.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.has.class_id               = true;
            p.header.packet_type                = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p.fields.class_id.packet_class_code = i % 4;
        });
        common::generate_packet_sequence("var_class_id.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.has.class_id                    = true;
            p.header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p.fields.class_id.oui                    = i % 2;
            p.fields.class_id.information_class_code = i % 2;
            p.fields.class_id.packet_class_code      = i % 2;
        });
        common::generate_packet_sequence("var_def_class_id.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.has.class_id                    = i % 4 != 0;
            p.header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p.fields.class_id.oui                    = i % 2;
            p.fields.class_id.information_class_code = i % 2;
            p.fields.class_id.packet_class_code      = i % 2;
        });
        common::generate_packet_sequence("var_class_stream_id.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.has.class_id                    = true;
            p.header.packet_type                     = VRT_PT_IF_DATA_WITH_STREAM_ID;
            p.fields.class_id.oui                    = i % 2;
            p.fields.class_id.information_class_code = i % 2;
            p.fields.class_id.packet_class_code      = i % 2;
            p.fields.stream_id                       = i % 4;
        });
        common::generate_packet_sequence("var_def_class_stream_id.vrt", &p, N_PACKETS, [&](uint64_t i) {
            if (i % 3 == 0) {
                p.header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
                p.fields.stream_id   = i % 4;
            } else {
                p.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
            }
            if (i % 6 == 0) {
                p.header.has.class_id = false;
            } else {
                p.header.has.class_id                    = true;
                p.fields.class_id.oui                    = i % 2;
                p.fields.class_id.information_class_code = i % 2;
                p.fields.class_id.packet_class_code      = i % 2;
            }
        });
        common::generate_packet_sequence("var_hex.vrt", &p, N_PACKETS, [&](uint64_t i) {
            p.header.has.class_id = true;
            p.header.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
            if (i % 2 == 0) {
                p.fields.class_id.oui                    = 0xBAAAAD;
                p.fields.class_id.information_class_code = 0x4B1D;
                p.fields.class_id.packet_class_code      = 0xDEAD;
                p.fields.stream_id                       = 0xDEADBEEF;
            } else {
                p.fields.class_id.oui                    = 0xABABAB;
                p.fields.class_id.information_class_code = 0xBEBE;
                p.fields.class_id.packet_class_code      = 0xDEDE;
                p.fields.stream_id                       = 0xFEFEFEFE;
            }
        });
    } catch (const std::runtime_error& exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
