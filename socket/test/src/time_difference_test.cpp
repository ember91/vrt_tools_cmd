#include <gtest/gtest.h>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_init.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

#include "../../src/time_difference.h"

using namespace vrt::socket;
using PacketPtr = std::shared_ptr<vrt_packet>;

static const int64_t NS_IN_S{1000 * 1000 * 1000};
static const int64_t PS_IN_S{1000 * NS_IN_S};

class TimeDifferenceTest : public ::testing::Test {
   protected:
    TimeDifferenceTest() {}

    void SetUp() override {
        p1_ = std::make_shared<vrt_packet>();
        p2_ = std::make_shared<vrt_packet>();
        vrt_init_packet(p1_.get());
        vrt_init_packet(p2_.get());
    }

    PacketPtr      p1_;
    PacketPtr      p2_;
    TimeDifference td_;
};

TEST_F(TimeDifferenceTest, None) {
    TimeDifference td;

    ASSERT_EQ(td.calculate(p1_).count(), 0);
    ASSERT_EQ(td.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, Utc) {
    p1_->header.tsi                       = VRT_TSI_UTC;
    p1_->fields.integer_seconds_timestamp = 10000;
    p2_->header.tsi                       = p1_->header.tsi;
    p2_->fields.integer_seconds_timestamp = p1_->fields.integer_seconds_timestamp + 3;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 3 * NS_IN_S);
}

TEST_F(TimeDifferenceTest, UtcNegative) {
    p1_->header.tsi                       = VRT_TSI_UTC;
    p1_->fields.integer_seconds_timestamp = 10000;
    p2_->header.tsi                       = p1_->header.tsi;
    p2_->fields.integer_seconds_timestamp = p1_->fields.integer_seconds_timestamp - 1;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, Gps) {
    p1_->header.tsi                       = VRT_TSI_GPS;
    p1_->fields.integer_seconds_timestamp = 10000;
    p2_->header.tsi                       = p1_->header.tsi;
    p2_->fields.integer_seconds_timestamp = p1_->fields.integer_seconds_timestamp + 3;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 3 * NS_IN_S);
}

TEST_F(TimeDifferenceTest, GpsNegative) {
    p1_->header.tsi                       = VRT_TSI_GPS;
    p1_->fields.integer_seconds_timestamp = 10000;
    p2_->header.tsi                       = p1_->header.tsi;
    p2_->fields.integer_seconds_timestamp = p1_->fields.integer_seconds_timestamp - 1;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, Other) {
    p1_->header.tsi                       = VRT_TSI_OTHER;
    p1_->fields.integer_seconds_timestamp = 10000;
    p2_->header.tsi                       = p1_->header.tsi;
    p2_->fields.integer_seconds_timestamp = p1_->fields.integer_seconds_timestamp + 3;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 3 * NS_IN_S);
}

TEST_F(TimeDifferenceTest, OtherNegative) {
    p1_->header.tsi                       = VRT_TSI_OTHER;
    p1_->fields.integer_seconds_timestamp = 10000;
    p2_->header.tsi                       = p1_->header.tsi;
    p2_->fields.integer_seconds_timestamp = p1_->fields.integer_seconds_timestamp - 1;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, SampleCountWithoutSampleRate) {
    p1_->header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp + 1000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, SampleCountWithoutSampleRateNegative) {
    p1_->header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp - 1000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, SampleCountWithSampleRate) {
    td_.set_sample_rate(16000.0);
    p1_->header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp + 8000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, SampleCountWithSampleRateNegative) {
    td_.set_sample_rate(16000.0);
    p1_->header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp - 8000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, RealTime) {
    p1_->header.tsf                          = VRT_TSF_REAL_TIME;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp + PS_IN_S / 2;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, RealTimeNegative) {
    p1_->header.tsf                          = VRT_TSF_REAL_TIME;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp - PS_IN_S / 2;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, FreeRunningCountWithoutSampleRate) {
    p1_->header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp + 8000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, FreeRunningCountWithoutSampleRateNegative) {
    p1_->header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp - 8000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 0);
}

TEST_F(TimeDifferenceTest, FreeRunningCountWithSampleRate) {
    td_.set_sample_rate(16000.0);
    p1_->header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp + 8000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), NS_IN_S / 2);
}

TEST_F(TimeDifferenceTest, UtcSampleCountWithoutSampleRate) {
    p1_->header.tsi                          = VRT_TSI_UTC;
    p1_->header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_->fields.integer_seconds_timestamp    = 10000;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsi                          = p1_->header.tsi;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.integer_seconds_timestamp    = p1_->fields.integer_seconds_timestamp + 3;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp + 8000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 3 * NS_IN_S);
}

TEST_F(TimeDifferenceTest, UtcSampleCountWithSampleRate) {
    td_.set_sample_rate(16000.0);
    p1_->header.tsi                          = VRT_TSI_UTC;
    p1_->header.tsf                          = VRT_TSF_SAMPLE_COUNT;
    p1_->fields.integer_seconds_timestamp    = 10000;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsi                          = p1_->header.tsi;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.integer_seconds_timestamp    = p1_->fields.integer_seconds_timestamp + 3;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp - 8000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 3 * NS_IN_S - NS_IN_S / 2);
}

TEST_F(TimeDifferenceTest, UtcRealTime) {
    p1_->header.tsi                          = VRT_TSI_UTC;
    p1_->header.tsf                          = VRT_TSF_REAL_TIME;
    p1_->fields.integer_seconds_timestamp    = 10000;
    p1_->fields.fractional_seconds_timestamp = PS_IN_S * 3 / 4;
    p2_->header.tsi                          = p1_->header.tsi;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.integer_seconds_timestamp    = p1_->fields.integer_seconds_timestamp + 3;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp - PS_IN_S / 2;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 3 * NS_IN_S - NS_IN_S / 2);
}

TEST_F(TimeDifferenceTest, UtcFreeRunningCountWithoutSampleRate) {
    p1_->header.tsi                          = VRT_TSI_UTC;
    p1_->header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_->fields.integer_seconds_timestamp    = 10000;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsi                          = p1_->header.tsi;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.integer_seconds_timestamp    = p1_->fields.integer_seconds_timestamp + 3;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp + PS_IN_S / 2;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), 3 * NS_IN_S);
}

TEST_F(TimeDifferenceTest, UtcFreeRunningCountWithSampleRate) {
    td_.set_sample_rate(16000.0);
    p1_->header.tsi                          = VRT_TSI_UTC;
    p1_->header.tsf                          = VRT_TSF_FREE_RUNNING_COUNT;
    p1_->fields.integer_seconds_timestamp    = 10000;
    p1_->fields.fractional_seconds_timestamp = 10000;
    p2_->header.tsi                          = p1_->header.tsi;
    p2_->header.tsf                          = p1_->header.tsf;
    p2_->fields.integer_seconds_timestamp    = p1_->fields.integer_seconds_timestamp + 3;
    p2_->fields.fractional_seconds_timestamp = p1_->fields.fractional_seconds_timestamp + 8000;

    ASSERT_EQ(td_.calculate(p1_).count(), 0);
    ASSERT_EQ(td_.calculate(p2_).count(), NS_IN_S / 2);
}
