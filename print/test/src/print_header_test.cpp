#include <gtest/gtest.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>

#include "../../src/type_printer.h"

class PrintHeaderTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_packet(&p_); }

    void TearDown() override { vrt::print::print_header(p_); }

    vrt_packet p_;
};

TEST_F(PrintHeaderTest, None) {}

TEST_F(PrintHeaderTest, PacketType1) {
    p_.header.packet_type = VRT_PT_EXT_CONTEXT;
}

TEST_F(PrintHeaderTest, PacketType2) {
    p_.header.packet_type = static_cast<vrt_packet_type>(0xFFFFFFFF);
}

TEST_F(PrintHeaderTest, HasClassId) {
    p_.header.has.class_id = true;
}

TEST_F(PrintHeaderTest, HasTrailer) {
    p_.header.has.trailer = true;
}

TEST_F(PrintHeaderTest, Tsm) {
    p_.header.tsm = VRT_TSM_COARSE;
}

TEST_F(PrintHeaderTest, Tsi1) {
    p_.header.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintHeaderTest, Tsi2) {
    p_.header.tsi = static_cast<vrt_tsi>(0xFFFFFFFF);
}

TEST_F(PrintHeaderTest, Tsf1) {
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintHeaderTest, Tsf2) {
    p_.header.tsf = static_cast<vrt_tsf>(0xFFFFFFFF);
}

TEST_F(PrintHeaderTest, PacketCount) {
    p_.header.packet_count = 0xFF;
}

TEST_F(PrintHeaderTest, PacketSize) {
    p_.header.packet_size = 0xFEDC;
}

TEST_F(PrintHeaderTest, EveryOther1) {
    p_.header.packet_type  = VRT_PT_IF_CONTEXT;
    p_.header.has.class_id = false;
    p_.header.has.trailer  = true;
    p_.header.tsm          = VRT_TSM_FINE;
    p_.header.tsi          = VRT_TSI_UTC;
    p_.header.tsf          = VRT_TSF_NONE;
    p_.header.packet_count = 0xF;
    p_.header.packet_size  = 0x0000;
}

TEST_F(PrintHeaderTest, EveryOther2) {
    p_.header.packet_type  = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    p_.header.has.class_id = true;
    p_.header.has.trailer  = true;
    p_.header.tsm          = VRT_TSM_FINE;
    p_.header.tsi          = VRT_TSI_NONE;
    p_.header.tsf          = VRT_TSF_SAMPLE_COUNT;
    p_.header.packet_count = 0x0;
    p_.header.packet_size  = 0xFFFF;
}

TEST_F(PrintHeaderTest, All) {
    p_.header.packet_type  = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    p_.header.has.class_id = true;
    p_.header.has.trailer  = true;
    p_.header.tsm          = VRT_TSM_FINE;
    p_.header.tsi          = VRT_TSI_OTHER;
    p_.header.tsf          = VRT_TSF_SAMPLE_COUNT;
    p_.header.packet_count = 0xC;
    p_.header.packet_size  = 0xBA98;
}
