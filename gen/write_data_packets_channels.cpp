#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_write.h>

#include "generate_packet_sequence.h"

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
    std::array<float, SIZE - 10> s;
    for (int i{0}; i < s.size(); ++i) {
        s[i] = std::sin(2.0F * PI * CENTER_FREQUENCY * static_cast<float>(i) / SAMPLE_RATE);
    }

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
        vrt::generate_packet_sequence("var_stream_id.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
            p->header.has.class_id = false;
            p->fields.stream_id    = i % 4;
        });
        vrt::generate_packet_sequence("var_def_stream_id.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.has.class_id = false;
            if (i % 4 == 0) {
                p->header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            } else {
                p->header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
                p->fields.stream_id   = i % 4;
            }
        });
        vrt::generate_packet_sequence("var_oui.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.has.class_id = true;
            p->header.packet_type  = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.oui = i % 4;
        });
        vrt::generate_packet_sequence("var_icc.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.has.class_id                    = true;
            p->header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.information_class_code = i % 4;
        });
        vrt::generate_packet_sequence("var_pcc.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.has.class_id               = true;
            p->header.packet_type                = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.packet_class_code = i % 4;
        });
        vrt::generate_packet_sequence("var_class_id.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.has.class_id                    = true;
            p->header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.oui                    = i % 2;
            p->fields.class_id.information_class_code = i % 2;
            p->fields.class_id.packet_class_code      = i % 2;
        });
        vrt::generate_packet_sequence("var_def_class_id.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.has.class_id                    = i % 4 != 0;
            p->header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.oui                    = i % 2;
            p->fields.class_id.information_class_code = i % 2;
            p->fields.class_id.packet_class_code      = i % 2;
        });
        vrt::generate_packet_sequence("var_class_stream_id.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.has.class_id                    = true;
            p->header.packet_type                     = VRT_PT_IF_DATA_WITH_STREAM_ID;
            p->fields.class_id.oui                    = i % 2;
            p->fields.class_id.information_class_code = i % 2;
            p->fields.class_id.packet_class_code      = i % 2;
            p->fields.stream_id                       = i % 4;
        });
        vrt::generate_packet_sequence("var_def_class_stream_id.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            if (i % 3 == 0) {
                p->header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
                p->fields.stream_id   = i % 4;
            } else {
                p->header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
            }
            if (i % 6 == 0) {
                p->header.has.class_id = false;
            } else {
                p->header.has.class_id                    = true;
                p->fields.class_id.oui                    = i % 2;
                p->fields.class_id.information_class_code = i % 2;
                p->fields.class_id.packet_class_code      = i % 2;
            }
        });
        vrt::generate_packet_sequence("var_hex.vrt", &p, N_PACKETS, [](int i, vrt_packet* p) {
            p->header.has.class_id = true;
            p->header.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
            if (i % 2 == 0) {
                p->fields.class_id.oui                    = 0xBAAAAD;
                p->fields.class_id.information_class_code = 0x4B1D;
                p->fields.class_id.packet_class_code      = 0xDEAD;
                p->fields.stream_id                       = 0xDEADBEEF;
            } else {
                p->fields.class_id.oui                    = 0xABABAB;
                p->fields.class_id.information_class_code = 0xBEBE;
                p->fields.class_id.packet_class_code      = 0xDEDE;
                p->fields.stream_id                       = 0xFEFEFEFE;
            }
        });
    } catch (const std::runtime_error& exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
