#include <gtest/gtest.h>

#include "vrt/vrt_init.h"
#include "vrt/vrt_types.h"

#include "../../src/type_printer.h"

class PrintFieldsTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_packet(&p_); }

    void TearDown() override {
        vrt::print::print_header(p_);
        vrt::print::print_fields(p_, SAMPLE_RATE);
    }

    static constexpr double SAMPLE_RATE{16e6};

    vrt_packet p_;
};

TEST_F(PrintFieldsTest, None) {}

TEST_F(PrintFieldsTest, StreamIdIfDataWithout) {
    p_.header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    p_.fields.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdIfDataWith) {
    p_.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.fields.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdExtDataWithout) {
    p_.header.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    p_.fields.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdExtDataWith) {
    p_.header.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    p_.fields.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdIfContext) {
    p_.header.packet_type = VRT_PT_IF_CONTEXT;
    p_.fields.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, StreamIdExtContext) {
    p_.header.packet_type = VRT_PT_EXT_CONTEXT;
    p_.fields.stream_id   = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, ClassId) {
    p_.header.has.class_id                    = true;
    p_.fields.class_id.oui                    = 0xFFFFEDCB;
    p_.fields.class_id.information_class_code = 0xFEDC;
    p_.fields.class_id.packet_class_code      = 0xBA98;
}

TEST_F(PrintFieldsTest, IntegerSecondsTimestampUtc) {
    p_.header.tsi                       = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp = 0x1EDCBA98;
}

TEST_F(PrintFieldsTest, IntegerSecondsTimestampOther) {
    p_.header.tsi                       = VRT_TSI_OTHER;
    p_.fields.integer_seconds_timestamp = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, FractionalSecondsTimestampSampleCount) {
    p_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p_.fields.fractional_seconds_timestamp = 0xFEDCBA987654321F;
}

TEST_F(PrintFieldsTest, IntegerFractionalSecondsTimestampUtcSampleCount) {
    p_.header.tsi                          = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp    = 0x1EDCBA98;
    p_.header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p_.fields.fractional_seconds_timestamp = SAMPLE_RATE / 2;
}

TEST_F(PrintFieldsTest, IntegerFractionalSecondsTimestampUtcRealTime) {
    p_.header.tsi                          = VRT_TSI_UTC;
    p_.fields.integer_seconds_timestamp    = 0x1EDCBA98;
    p_.header.tsf                          = VRT_TSF_REAL_TIME;
    p_.fields.fractional_seconds_timestamp = 500000000000;
}

TEST_F(PrintFieldsTest, EveryOther1) {
    p_.header.packet_type               = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    p_.header.has.class_id              = false;
    p_.header.tsi                       = VRT_TSI_OTHER;
    p_.header.tsf                       = VRT_TSF_NONE;
    p_.header.packet_count              = 0xF;
    p_.fields.stream_id                 = 0xDEADBEEF;
    p_.fields.integer_seconds_timestamp = 0xFEDCBA98;
}

TEST_F(PrintFieldsTest, EveryOther2) {
    p_.header.packet_type                     = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
    p_.header.has.class_id                    = true;
    p_.header.tsi                             = VRT_TSI_NONE;
    p_.header.tsf                             = VRT_TSF_FREE_RUNNING_COUNT;
    p_.header.packet_count                    = 0x0;
    p_.fields.class_id.oui                    = 0x00FEDCBA;
    p_.fields.class_id.information_class_code = 0xFEDC;
    p_.fields.class_id.packet_class_code      = 0xBA98;
    p_.fields.fractional_seconds_timestamp    = 0xFEDCBA987654321F;
}

TEST_F(PrintFieldsTest, All) {
    p_.header.packet_type                     = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    p_.header.has.class_id                    = true;
    p_.header.tsi                             = VRT_TSI_OTHER;
    p_.header.tsf                             = VRT_TSF_SAMPLE_COUNT;
    p_.header.packet_count                    = 0xC;
    p_.fields.stream_id                       = 0xDEADBEEF;
    p_.fields.class_id.oui                    = 0x00FEDCBA;
    p_.fields.class_id.information_class_code = 0xFEDC;
    p_.fields.class_id.packet_class_code      = 0xBA98;
    p_.fields.integer_seconds_timestamp       = 0xFEDCBA98;
    p_.fields.fractional_seconds_timestamp    = 0xFEDCBA987654321F;
}
