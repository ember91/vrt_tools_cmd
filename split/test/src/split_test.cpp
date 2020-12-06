#include <gtest/gtest.h>
#include <filesystem>

#include <vrt/vrt_init.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_types.h>

#include "../../src/process.h"
#include "byte_swap.h"
#include "generate_packet_sequence.h"

namespace fs = std::filesystem;

static const size_t      N_PACKETS{100};
static const std::string TMP_DIR{"test_tmp"};
static const std::string TMP_FILE_NAME{"split.vrt"};
static const std::string TMP_FILE_PATH{TMP_DIR + "/" + TMP_FILE_NAME};

class SplitTest : public ::testing::Test {
   protected:
    SplitTest() : p_() {}

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
    };

    vrt_packet p_;
};

static void process(bool do_byte_swap = false) {
    vrt::split::ProgramArguments args;
    args.file_path_in = TMP_FILE_PATH;
    args.do_byte_swap = do_byte_swap;
    vrt::split::process(args);
}

static void compare(std::vector<std::string> file_names, bool do_byte_swap = false) {
    std::vector<uint32_t> buf;

    size_t total_packets{0};
    for (const auto& file_name : file_names) {
        std::string file_path = TMP_DIR + "/" + file_name;

        // Open file for reading
        std::ifstream file(file_path, std::ios::in | std::ios::binary);
        if (!file) {
            // Note that destructor is not run if this fails
            std::stringstream ss;
            ss << "Failed to open input file '" << file_path << "'";
            std::runtime_error(ss.str());
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
            file.read(reinterpret_cast<char*>(buf.data()), sizeof(uint32_t) * VRT_WORDS_HEADER);
            if (file.gcount() != sizeof(uint32_t) * VRT_WORDS_HEADER) {
                break;
            }

            if (do_byte_swap) {
                buf[0] = bswap_32(buf[0]);
            }

            // Parse and validate header
            vrt_header header;
            int32_t    words_header{vrt_read_header(buf.data(), buf.size(), &header, true)};
            ASSERT_GE(words_header, 0);

            // Resize buffer if needed
            if (buf.size() < header.packet_size) {
                buf.resize(header.packet_size);
            }

            // Read packet remainder
            file.read(reinterpret_cast<char*>(buf.data() + words_header),
                      sizeof(uint32_t) * (header.packet_size - words_header));
            if (file.gcount() != sizeof(uint32_t) * (header.packet_size - words_header)) {
                break;
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

            // Compare
            if (i != 0) {
                ASSERT_EQ(header.has.class_id, prev_header.has.class_id);
                if (header.has.class_id) {
                    ASSERT_EQ(fields.class_id.oui, prev_fields.class_id.oui);
                    ASSERT_EQ(fields.class_id.information_class_code, prev_fields.class_id.information_class_code);
                    ASSERT_EQ(fields.class_id.packet_class_code, prev_fields.class_id.packet_class_code);
                }
                ASSERT_EQ(header.packet_type, prev_header.packet_type);
                if (vrt_has_stream_id(&header)) {
                    ASSERT_EQ(fields.stream_id, fields.stream_id);
                }
            }

            prev_header = header;
            prev_fields = fields;
        }

        total_packets += i;
    }
    if (!file_names.empty()) {
        ASSERT_EQ(total_packets, N_PACKETS);
    }

    std::vector<fs::path> files;
    for (const auto& p : fs::directory_iterator(TMP_DIR)) {
        files.push_back(p.path());
    }

    ASSERT_EQ(files.size(), file_names.size() + 1);
}

TEST_F(SplitTest, None) {
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS);

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({});
}

TEST_F(SplitTest, SameClassId) {
    p_.header.has.class_id                    = true;
    p_.fields.class_id.oui                    = 0xDEDEDE;
    p_.fields.class_id.information_class_code = 0xBEBE;
    p_.fields.class_id.packet_class_code      = 0xABAB;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS);

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({});
}

TEST_F(SplitTest, SameStreamId) {
    p_.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.fields.stream_id   = 0xDEADBEEF;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS);

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({});
}

TEST_F(SplitTest, SameAll) {
    p_.header.packet_type                     = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.fields.stream_id                       = 0xDEADBEEF;
    p_.header.has.class_id                    = true;
    p_.fields.class_id.oui                    = 0xDEDEDE;
    p_.fields.class_id.information_class_code = 0xBEBE;
    p_.fields.class_id.packet_class_code      = 0xABAB;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS);

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({});
}

TEST_F(SplitTest, Oui) {
    p_.header.has.class_id = true;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](int i, vrt_packet* p) { p->fields.class_id.oui = i % 4; });

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_0.vrt", "split_1.vrt", "split_2.vrt", "split_3.vrt"});
}

TEST_F(SplitTest, InformationClassCode) {
    p_.header.has.class_id = true;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](int i, vrt_packet* p) { p->fields.class_id.information_class_code = i % 4; });

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_0.vrt", "split_1.vrt", "split_2.vrt", "split_3.vrt"});
}

TEST_F(SplitTest, PacketClassCode) {
    p_.header.has.class_id = true;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](int i, vrt_packet* p) { p->fields.class_id.packet_class_code = i % 4; });

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_0.vrt", "split_1.vrt", "split_2.vrt", "split_3.vrt"});
}

TEST_F(SplitTest, StreamId) {
    p_.header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS,
                                  [](int i, vrt_packet* p) { p->fields.stream_id = i % 4; });

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_0.vrt", "split_1.vrt", "split_2.vrt", "split_3.vrt"});
}

TEST_F(SplitTest, All) {
    p_.header.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.header.has.class_id = true;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](int i, vrt_packet* p) {
        p->fields.stream_id                       = i % 4;
        p->fields.class_id.oui                    = p->fields.stream_id;
        p->fields.class_id.information_class_code = p->fields.stream_id;
        p->fields.class_id.packet_class_code      = p->fields.stream_id;
    });

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_0_0_0_0.vrt", "split_1_1_1_1.vrt", "split_2_2_2_2.vrt", "split_3_3_3_3.vrt"});
}

TEST_F(SplitTest, SomeClassIdDefault) {
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](int i, vrt_packet* p) {
        p->header.has.class_id                    = i % 4 != 0;
        p->fields.class_id.oui                    = i % 2;
        p->fields.class_id.information_class_code = i % 2;
        p->fields.class_id.packet_class_code      = i % 2;
    });

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_0_0_0.vrt", "split_1_1_1.vrt", "split_X_X_X.vrt"});
}

TEST_F(SplitTest, SomeStreamIdDefault) {
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](int i, vrt_packet* p) {
        if (i % 4 == 0) {
            p->header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
        } else {
            p->header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
            p->fields.stream_id   = i % 4;
        }
    });

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_1.vrt", "split_2.vrt", "split_3.vrt", "split_X.vrt"});
}

TEST_F(SplitTest, SomeClassStreamIdDefault) {
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](int i, vrt_packet* p) {
        if (i % 3 == 0) {
            p->header.packet_type = VRT_PT_IF_DATA_WITHOUT_STREAM_ID;
        } else {
            p->header.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
            p->fields.stream_id   = i % 4;
        }
        if (i % 5 == 0) {
            p->header.has.class_id = false;
        } else {
            p->header.has.class_id                    = true;
            p->fields.class_id.oui                    = i % 2;
            p->fields.class_id.information_class_code = i % 2;
            p->fields.class_id.packet_class_code      = i % 2;
        }
    });

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_X_X_X_X.vrt", "split_0_0_0_X.vrt", "split_1_1_1_X.vrt", "split_X_X_X_0.vrt", "split_X_X_X_1.vrt",
             "split_X_X_X_2.vrt", "split_X_X_X_3.vrt", "split_0_0_0_0.vrt", "split_0_0_0_2.vrt", "split_1_1_1_1.vrt",
             "split_1_1_1_3.vrt"});
}

TEST_F(SplitTest, HexNaming) {
    p_.header.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.header.has.class_id = true;
    vrt::generate_packet_sequence(TMP_FILE_PATH, &p_, N_PACKETS, [](int i, vrt_packet* p) {
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

    process();
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_BAAAAD_4B1D_DEAD_DEADBEEF.vrt", "split_ABABAB_BEBE_DEDE_FEFEFEFE.vrt"});
}

TEST_F(SplitTest, ByteSwap) {
    p_.header.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
    p_.header.has.class_id = true;
    vrt::generate_packet_sequence(
        TMP_FILE_PATH, &p_, N_PACKETS,
        [](int i, vrt_packet* p) {
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
        },
        true);

    process(true);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    compare({"split_BAAAAD_4B1D_DEAD_DEADBEEF.vrt", "split_ABABAB_BEBE_DEDE_FEFEFEFE.vrt"}, true);
}
