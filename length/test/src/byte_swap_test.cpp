#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>

#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>

#include "../../src/process.h"
#include "../../src/program_arguments.h"
#include "common/byte_swap.h"
#include "common/generate_packet_sequence.h"

using namespace vrt;

namespace fs = ::std::filesystem;

static const size_t   N_PACKETS{10};
static const fs::path TMP_DIR{"test_tmp"};
static const fs::path TMP_FILE_NAME{"split.vrt"};
static const fs::path TMP_FILE_PATH{TMP_DIR / TMP_FILE_NAME};

class LengthTest : public ::testing::Test {
   protected:
    LengthTest() : p_() {}

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

static void process(bool do_byte_swap = false) {
    vrt::length::ProgramArguments args;
    args.file_path_in = TMP_FILE_PATH;
    args.do_byte_swap = do_byte_swap;
    vrt::length::process(args);
}

TEST_F(LengthTest, ByteSwap) {
    p_.header.tsi = VRT_TSI_UTC;
    common::generate_packet_sequence(
        TMP_FILE_PATH, &p_, N_PACKETS, [](uint64_t i, vrt_packet* p) { p->fields.integer_seconds_timestamp = i; },
        true);

    process(true);
}
