#include <gtest/gtest.h>

#include <vrt/vrt_init.h>

#include "../../src/type_printer.h"

class PrintHeaderTest : public ::testing::Test {
   protected:
    PrintHeaderTest() : h_() {}

    void SetUp() override { vrt_init_header(&h_); }

    void TearDown() override { vrt::print::print_header(h_); }

    vrt_header h_;
};

TEST_F(PrintHeaderTest, None) {}

TEST_F(PrintHeaderTest, PacketType1) {
    h_.packet_type = VRT_PT_EXT_CONTEXT;
}

TEST_F(PrintHeaderTest, PacketType2) {
    h_.packet_type = static_cast<vrt_packet_type>(0xFFFFFFFF);
}

TEST_F(PrintHeaderTest, HasClassId) {
    h_.has.class_id = true;
}

TEST_F(PrintHeaderTest, HasTrailer) {
    h_.has.trailer = true;
}

TEST_F(PrintHeaderTest, Tsm) {
    h_.tsm = VRT_TSM_COARSE;
}

TEST_F(PrintHeaderTest, Tsi1) {
    h_.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintHeaderTest, Tsi2) {
    h_.tsi = static_cast<vrt_tsi>(0xFFFFFFFF);
}

TEST_F(PrintHeaderTest, Tsf1) {
    h_.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintHeaderTest, Tsf2) {
    h_.tsf = static_cast<vrt_tsf>(0xFFFFFFFF);
}

TEST_F(PrintHeaderTest, PacketCount) {
    h_.packet_count = 0xFF;
}

TEST_F(PrintHeaderTest, PacketSize) {
    h_.packet_size = 0xFEDC;
}

TEST_F(PrintHeaderTest, EveryOther1) {
    h_.packet_type  = VRT_PT_IF_CONTEXT;
    h_.has.class_id = false;
    h_.has.trailer  = true;
    h_.tsm          = VRT_TSM_FINE;
    h_.tsi          = VRT_TSI_UTC;
    h_.tsf          = VRT_TSF_NONE;
    h_.packet_count = 0xF;
    h_.packet_size  = 0x0000;
}

TEST_F(PrintHeaderTest, EveryOther2) {
    h_.packet_type  = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    h_.has.class_id = true;
    h_.has.trailer  = true;
    h_.tsm          = VRT_TSM_FINE;
    h_.tsi          = VRT_TSI_NONE;
    h_.tsf          = VRT_TSF_SAMPLE_COUNT;
    h_.packet_count = 0x0;
    h_.packet_size  = 0xFFFF;
}

TEST_F(PrintHeaderTest, All) {
    h_.packet_type  = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    h_.has.class_id = true;
    h_.has.trailer  = true;
    h_.tsm          = VRT_TSM_FINE;
    h_.tsi          = VRT_TSI_OTHER;
    h_.tsf          = VRT_TSF_SAMPLE_COUNT;
    h_.packet_count = 0xC;
    h_.packet_size  = 0xBA98;
}
