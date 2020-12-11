#include <gtest/gtest.h>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_init.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

#include "../../src/process.h"
#include "../../src/program_arguments.h"
#include "byte_swap.h"
#include "generate_packet_sequence.h"

namespace fs = ::std::filesystem;

static const size_t   N_PACKETS{100};
static const fs::path TMP_DIR{"test_tmp"};
static const fs::path TMP_FILE_OUT_PATH{TMP_DIR / "merge.vrt"};

class MergeTest : public ::testing::Test {
   protected:
    MergeTest() : p_() {}

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

static std::vector<fs::path> generate_input_file_paths(size_t n) {
    std::vector<fs::path> paths;
    paths.reserve(n);

    for (size_t i{0}; i < n; ++i) {
        fs::path file_path{TMP_DIR};
        file_path /= "merge_";
        file_path += std::to_string(i);
        file_path += ".vrt";
        paths.push_back(file_path);
    }

    return paths;
}

static void process(const std::vector<fs::path>& file_paths_in, bool do_byte_swap = false) {
    vrt::merge::ProgramArguments args;
    args.file_paths_in = file_paths_in;
    args.file_path_out = TMP_FILE_OUT_PATH;
    args.do_byte_swap  = do_byte_swap;
    vrt::merge::process(args);
}

static void check(bool do_byte_swap = false) {
    std::vector<uint32_t> buf;

    fs::path file_path(TMP_FILE_OUT_PATH);

    std::ifstream file;
    file.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);

    // Open file for reading
    try {
        file.open(file_path, std::ios::in | std::ios::binary);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to open input file '" << file_path << "'";
        throw std::runtime_error(ss.str());
    }

    vrt_header prev_header;
    vrt_fields prev_fields;

    size_t i;
    for (i = 0;; ++i) {
        // Make space for header
        if (buf.size() < VRT_WORDS_HEADER) {
            buf.resize(VRT_WORDS_HEADER);
        }

        // Read header
        // We know here that buffer size at least has room for the header
        try {
            file.read(reinterpret_cast<char*>(buf.data()), sizeof(uint32_t) * VRT_WORDS_HEADER);
        } catch (const std::ios::failure&) {
            if (file.eof()) {
                break;
            }
            throw std::runtime_error("File read error");
        }

        if (do_byte_swap) {
            buf[0] = bswap_32(buf[0]);
        }

        // Parse and validate header
        vrt_header header;
        int32_t    words_header{vrt_read_header(buf.data(), buf.size(), &header, true)};
        ASSERT_GT(words_header, 0);

        // Resize buffer if needed
        if (buf.size() < header.packet_size) {
            buf.resize(header.packet_size);
        }

        // Read packet remainder
        try {
            file.read(reinterpret_cast<char*>(buf.data() + words_header),
                      sizeof(uint32_t) * (header.packet_size - words_header));
        } catch (const std::ios::failure&) {
            if (file.eof()) {
                break;
            }
            throw std::runtime_error("File read error");
        }

        // Byte swap remainder of packet
        if (do_byte_swap) {
            for (uint16_t j{1}; j < header.packet_size; ++j) {
                buf[j] = bswap_32(buf[j]);
            }
        }

        // Parse and validate fields section
        vrt_fields fields;
        int32_t    words_fields{
            vrt_read_fields(&header, buf.data() + words_header, buf.size() - words_header, &fields, true)};
        ASSERT_GE(words_fields, 0);

        if (i != 0) {
            ASSERT_EQ(header.tsi, prev_header.tsi);
            ASSERT_EQ(header.tsf, prev_header.tsf);
            ASSERT_GE(fields.integer_seconds_timestamp, prev_fields.integer_seconds_timestamp);
            if (fields.integer_seconds_timestamp > prev_fields.integer_seconds_timestamp) {
                ASSERT_LT(fields.fractional_seconds_timestamp, prev_fields.fractional_seconds_timestamp);
            } else {
                ASSERT_GE(fields.fractional_seconds_timestamp, prev_fields.fractional_seconds_timestamp);
            }
        }

        prev_header = header;
        prev_fields = fields;
    }

    ASSERT_EQ(i, N_PACKETS);
}

TEST_F(MergeTest, Sorted) {
    const uint64_t ps_in_s{1000000000000};
    const size_t   n{4};

    p_.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.header.tsi         = VRT_TSI_UTC;
    p_.header.tsf         = VRT_TSF_REAL_TIME;

    std::random_device                      rd;
    std::mt19937                            gen(rd());
    std::uniform_int_distribution<uint64_t> distrib(1, ps_in_s / 10);

    std::vector<fs::path> file_paths_in{generate_input_file_paths(n)};

    for (const auto& file_path : file_paths_in) {
        vrt::generate_packet_sequence(file_path, &p_, N_PACKETS / n, [&](uint64_t i, vrt_packet* p) {
            p->fields.stream_id = static_cast<uint32_t>(i);
            p->fields.fractional_seconds_timestamp += distrib(gen);
            if (p->fields.fractional_seconds_timestamp >= ps_in_s) {
                uint64_t t{p->fields.fractional_seconds_timestamp / ps_in_s};
                p->fields.integer_seconds_timestamp += t;
                p->fields.fractional_seconds_timestamp -= t * ps_in_s;
            }
        });
    }

    process(file_paths_in);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    check();
}

TEST_F(MergeTest, ByteSwap) {
    const uint64_t ps_in_s{1000000000000};
    const size_t   n{4};

    p_.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.header.tsi         = VRT_TSI_UTC;
    p_.header.tsf         = VRT_TSF_REAL_TIME;

    std::random_device                      rd;
    std::mt19937                            gen(rd());
    std::uniform_int_distribution<uint64_t> distrib(1, ps_in_s / 10);

    std::vector<fs::path> file_paths_in{generate_input_file_paths(n)};

    for (const auto& file_path : file_paths_in) {
        vrt::generate_packet_sequence(
            file_path, &p_, N_PACKETS / n,
            [&](uint64_t i, vrt_packet* p) {
                p->fields.stream_id = static_cast<uint32_t>(i);
                p->fields.fractional_seconds_timestamp += distrib(gen);
                if (p->fields.fractional_seconds_timestamp >= ps_in_s) {
                    uint64_t t{p->fields.fractional_seconds_timestamp / ps_in_s};
                    p->fields.integer_seconds_timestamp += t;
                    p->fields.fractional_seconds_timestamp -= t * ps_in_s;
                }
            },
            true);
    }

    process(file_paths_in, true);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    check(true);
}
