#include <gtest/gtest.h>

#include <array>
#include <cstdint>

#include <vrt/vrt_init.h>
#include <vrt/vrt_write.h>

#include "../../src/type_printer.h"

class PrintFieldsTest : public ::testing::Test {
   protected:
    PrintFieldsTest() : h_(), f_() {}

    void SetUp() override {
        vrt_init_header(&h_);
        vrt_init_fields(&f_);
    }

    void TearDown() override {
        vrt::print::print_header(h_);
        vrt::print::print_fields(h_, f_);
    }

    vrt_header h_;
    vrt_fields f_;
};

TEST_F(PrintFieldsTest, None) {}

TEST_F(PrintFieldsTest, StreamIdIfDataWithout) {
    h_.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    f_.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdIfDataWith) {
    h_.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    f_.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdExtDataWithout) {
    h_.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    f_.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdExtDataWith) {
    h_.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    f_.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdIfContext) {
    h_.packet_type = VRT_PT_IF_CONTEXT;
    f_.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdExtContext) {
    h_.packet_type = VRT_PT_EXT_CONTEXT;
    f_.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, ClassId) {
    h_.has.class_id                    = true;
    f_.class_id.oui                    = 0xFFFFEDCB;
    f_.class_id.information_class_code = 0xFEDC;
    f_.class_id.packet_class_code      = 0xBA98;
}

TEST_F(PrintFieldsTest, IntegerSecondsTimestamp) {
    h_.tsi                       = VRT_TSI_OTHER;
    f_.integer_seconds_timestamp = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, FractionalSecondsTimestamp) {
    h_.tsf                          = VRT_TSF_SAMPLE_COUNT;
    f_.fractional_seconds_timestamp = 0xFEDCBA987654321F;
}

TEST_F(PrintFieldsTest, EveryOther1) {
    h_.packet_type               = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    h_.has.class_id              = false;
    h_.tsi                       = VRT_TSI_OTHER;
    h_.tsf                       = VRT_TSF_NONE;
    h_.packet_count              = 0xF;
    f_.stream_id                 = 0xDEADBEEF;
    f_.integer_seconds_timestamp = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, EveryOther2) {
    h_.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    h_.has.class_id                    = true;
    h_.tsi                             = VRT_TSI_NONE;
    h_.tsf                             = VRT_TSF_FREE_RUNNING_COUNT;
    h_.packet_count                    = 0x0;
    f_.class_id.oui                    = 0x00FEDCBA;
    f_.class_id.information_class_code = 0xFEDC;
    f_.class_id.packet_class_code      = 0xBA98;
    f_.fractional_seconds_timestamp    = 0xFEDCBA987654321F;
}

TEST_F(PrintFieldsTest, All) {
    h_.packet_type                     = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    h_.has.class_id                    = true;
    h_.tsi                             = VRT_TSI_OTHER;
    h_.tsf                             = VRT_TSF_SAMPLE_COUNT;
    h_.packet_count                    = 0xC;
    f_.stream_id                       = 0xDEADBEEF;
    f_.class_id.oui                    = 0x00FEDCBA;
    f_.class_id.information_class_code = 0xFEDC;
    f_.class_id.packet_class_code      = 0xBA98;
    f_.integer_seconds_timestamp       = 0xFEDCBA98;
    f_.fractional_seconds_timestamp    = 0xFEDCBA987654321F;
}
