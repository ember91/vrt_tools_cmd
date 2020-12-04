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

/* Size of packet in 32-bit words */
static const size_t SIZE{515};
/* Sample rate [Hz] */
static const float SAMPLE_RATE{44100.0F};
/* Center frequency [Hz] */
static const float CENTER_FREQUENCY{10000.0F};
/* M_PI in cmath is nonstandard :( */
static const float PI{3.1415926F};

void write(const std::string&                             file_path,
           vrt_packet*                                    p,
           uint32_t*                                      b,
           const std::function<void(int i, vrt_packet*)>& change) {
    /* Open file */
    std::ofstream fs(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!fs) {
        std::stringstream ss;
        ss << "Failed to open file '" << file_path << "'";
        throw std::runtime_error(ss.str());
    }
    for (int i{0}; i < 100; ++i) {
        /* Change fields depending on index */
        change(i, p);

        /* Write packet */
        int32_t size{vrt_write_packet(p, b, SIZE, true)};
        if (size < 0) {
            std::stringstream ss;
            ss << "Failed to write packet: " << vrt_string_error(size);
            throw std::runtime_error(ss.str());
        }

        /* Write generated packet to file */
        fs.write(reinterpret_cast<char*>(b), sizeof(uint32_t) * size);
        if (!fs) {
            std::stringstream ss;
            ss << "Failed to write to file '" << file_path << "'";
            throw std::runtime_error(ss.str());
        }
    }

    /* Cleanup */
    fs.close();
}

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

    try {
        write("var_stream_id.vrt", &p, b.data(), [](int i, vrt_packet* p) {
            p->header.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
            p->header.has.class_id = false;
            p->fields.stream_id    = i % 4;
        });
        write("var_def_stream_id.vrt", &p, b.data(), [](int i, vrt_packet* p) {
            p->header.has.class_id = false;
            if (i % 4 == 0) {
                p->header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            } else {
                p->header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
                p->fields.stream_id   = i % 4;
            }
        });
        write("var_oui.vrt", &p, b.data(), [](int i, vrt_packet* p) {
            p->header.has.class_id = true;
            p->header.packet_type  = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.oui = i % 4;
        });
        write("var_icc.vrt", &p, b.data(), [](int i, vrt_packet* p) {
            p->header.has.class_id                    = true;
            p->header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.information_class_code = i % 4;
        });
        write("var_pcc.vrt", &p, b.data(), [](int i, vrt_packet* p) {
            p->header.has.class_id               = true;
            p->header.packet_type                = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.packet_class_code = i % 4;
        });
        write("var_class_id.vrt", &p, b.data(), [](int i, vrt_packet* p) {
            p->header.has.class_id                    = true;
            p->header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.oui                    = i % 2;
            p->fields.class_id.information_class_code = i % 2;
            p->fields.class_id.packet_class_code      = i % 2;
        });
        write("var_def_class_id.vrt", &p, b.data(), [](int i, vrt_packet* p) {
            p->header.has.class_id                    = i % 4 != 0;
            p->header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
            p->fields.class_id.oui                    = i % 2;
            p->fields.class_id.information_class_code = i % 2;
            p->fields.class_id.packet_class_code      = i % 2;
        });
        write("var_class_stream_id.vrt", &p, b.data(), [](int i, vrt_packet* p) {
            p->header.has.class_id                    = true;
            p->header.packet_type                     = VRT_PT_IF_DATA_WITH_STREAM_ID;
            p->fields.class_id.oui                    = i % 2;
            p->fields.class_id.information_class_code = i % 2;
            p->fields.class_id.packet_class_code      = i % 2;
            p->fields.stream_id                       = i % 4;
        });
        write("var_def_class_stream_id.vrt", &p, b.data(), [](int i, vrt_packet* p) {
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
        write("var_hex.vrt", &p, b.data(), [](int i, vrt_packet* p) {
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
