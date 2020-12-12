#include <gtest/gtest.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_init.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>
#include <vrt/vrt_words.h>

#include "../../src/process.h"
#include "../../src/program_arguments.h"
#include "common/byte_swap.h"
#include "common/generate_packet_sequence.h"

namespace fs = ::std::filesystem;

static const size_t   N_PACKETS{10};
static const fs::path TMP_DIR{"test_tmp"};
static const fs::path TMP_FILE_NAME{"split.vrt"};
static const fs::path TMP_FILE_PATH{TMP_DIR / TMP_FILE_NAME};

class TimestampTest : public ::testing::Test {
   protected:
    TimestampTest() : p_() {}

    void SetUp() override {
        try {
            fs::remove_all(TMP_DIR);
        } catch (const fs::filesystem_error&) {
            // Do nothing
        }
        fs::create_directory(TMP_DIR);
        vrt_init_packet(&p_);
    }
    void TearDown() override {
        try {
            fs::remove_all(TMP_DIR);
        } catch (const fs::filesystem_error&) {
            // Do nothing
        }
    }

    vrt_packet p_;
};

static void process(double sample_rate = 0.0) {
    vrt::length::ProgramArguments args;
    args.file_path_in = TMP_FILE_PATH;
    args.sample_rate  = sample_rate;
    vrt::length::process(args);
}

TEST_F(TimestampTest, None) {
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, 0);

    process();
}

TEST_F(TimestampTest, TsiUtc) {
    p_.header.tsi = VRT_TSI_UTC;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](uint64_t i, vrt_packet* p) { p->fields.integer_seconds_timestamp = i; });

    process();
}

TEST_F(TimestampTest, TsiGps) {
    p_.header.tsi = VRT_TSI_GPS;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](uint64_t i, vrt_packet* p) { p->fields.integer_seconds_timestamp = i; });

    process();
}

TEST_F(TimestampTest, TsiOther) {
    p_.header.tsi = VRT_TSI_OTHER;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](uint64_t i, vrt_packet* p) { p->fields.integer_seconds_timestamp = i; });

    process();
}

TEST_F(TimestampTest, TsfSampleCount) {
    p_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](uint64_t i, vrt_packet* p) { p->fields.fractional_seconds_timestamp = i; });

    process();
}

TEST_F(TimestampTest, TsfRealTime) {
    p_.header.tsf = VRT_TSF_REAL_TIME;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](uint64_t i, vrt_packet* p) { p->fields.fractional_seconds_timestamp = i; });

    process();
}

TEST_F(TimestampTest, TsfFreeRunning) {
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](uint64_t i, vrt_packet* p) { p->fields.fractional_seconds_timestamp = i; });

    process();
}

TEST_F(TimestampTest, TsiUtcTsfSampleCount) {
    p_.header.tsi = VRT_TSI_UTC;
    p_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiUtcTsfRealTime) {
    p_.header.tsi = VRT_TSI_UTC;
    p_.header.tsf = VRT_TSF_REAL_TIME;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiUtcTsfFreeRunning) {
    p_.header.tsi = VRT_TSI_UTC;
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiGpsTsfSampleCount) {
    p_.header.tsi = VRT_TSI_GPS;
    p_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiGpsTsfRealTime) {
    p_.header.tsi = VRT_TSI_GPS;
    p_.header.tsf = VRT_TSF_REAL_TIME;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiGpsTsfFreeRunning) {
    p_.header.tsi = VRT_TSI_GPS;
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiOtherTsfSampleCount) {
    p_.header.tsi = VRT_TSI_OTHER;
    p_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiOtherTsfRealTime) {
    p_.header.tsi = VRT_TSI_OTHER;
    p_.header.tsf = VRT_TSF_REAL_TIME;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiOtherTsfFreeRunning) {
    p_.header.tsi = VRT_TSI_OTHER;
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process();
}

TEST_F(TimestampTest, TsiUtcTsfSampleCountSampleRate) {
    p_.header.tsi = VRT_TSI_UTC;
    p_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}

TEST_F(TimestampTest, TsiUtcTsfRealTimeSampleRate) {
    p_.header.tsi = VRT_TSI_UTC;
    p_.header.tsf = VRT_TSF_REAL_TIME;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}

TEST_F(TimestampTest, TsiUtcTsfFreeRunningSampleRate) {
    p_.header.tsi = VRT_TSI_UTC;
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}

TEST_F(TimestampTest, TsiGpsTsfSampleCountSampleRate) {
    p_.header.tsi = VRT_TSI_GPS;
    p_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}

TEST_F(TimestampTest, TsiGpsTsfRealTimeSampleRate) {
    p_.header.tsi = VRT_TSI_GPS;
    p_.header.tsf = VRT_TSF_REAL_TIME;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}

TEST_F(TimestampTest, TsiGpsTsfFreeRunningSampleRate) {
    p_.header.tsi = VRT_TSI_GPS;
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}

TEST_F(TimestampTest, TsiOtherTsfSampleCountSampleRate) {
    p_.header.tsi = VRT_TSI_OTHER;
    p_.header.tsf = VRT_TSF_SAMPLE_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}

TEST_F(TimestampTest, TsiOtherTsfRealTimeSampleRate) {
    p_.header.tsi = VRT_TSI_OTHER;
    p_.header.tsf = VRT_TSF_REAL_TIME;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}

TEST_F(TimestampTest, TsiOtherTsfFreeRunningSampleRate) {
    p_.header.tsi = VRT_TSI_OTHER;
    p_.header.tsf = VRT_TSF_FREE_RUNNING_COUNT;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) {
        p->fields.integer_seconds_timestamp    = i;
        p->fields.fractional_seconds_timestamp = i;
    });

    process(20.0);
}
