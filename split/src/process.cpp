#include "process.h"

#include <algorithm>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <vrt/vrt_read.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>

#include "byte_swap.h"
#include "output_file.h"
#include "program_arguments.h"

namespace fs = std::filesystem;

namespace vrt::split {

// For convenience
using PacketPtr     = std::shared_ptr<vrt_packet>;
using OutputFilePtr = std::unique_ptr<OutputFile>;

/**
 * Comparator to compare packets by Class and Stream ID.
 */
struct ComparatorId {
    /**
     * Compare packets by Class and Stream ID.
     *
     * \param a Packet 1.
     * \param b Packet 2.
     *
     * \return True if a is less than b.
     */
    bool operator()(const PacketPtr& a, const PacketPtr& b) const {
        if (!a->header.has.class_id && b->header.has.class_id) {
            return true;
        }
        if (a->header.has.class_id && !b->header.has.class_id) {
            return false;
        }
        // Either both or none has Class ID here
        if (a->header.has.class_id && b->header.has.class_id) {
            if (a->fields.class_id.oui < b->fields.class_id.oui) {
                return true;
            }
            if (a->fields.class_id.oui > b->fields.class_id.oui) {
                return false;
            }
            // OUI equal
            if (a->fields.class_id.information_class_code < b->fields.class_id.information_class_code) {
                return true;
            }
            if (a->fields.class_id.information_class_code > b->fields.class_id.information_class_code) {
                return false;
            }
            // OUI and Information class code equal
            if (a->fields.class_id.packet_class_code < b->fields.class_id.packet_class_code) {
                return true;
            }
            if (a->fields.class_id.packet_class_code > b->fields.class_id.packet_class_code) {
                return false;
            }
            // OUI, Information class code and Packet class code are equal
        }
        if (!vrt_has_stream_id(&a->header) && vrt_has_stream_id(&b->header)) {
            return true;
        }
        if (vrt_has_stream_id(&a->header) && !vrt_has_stream_id(&b->header)) {
            return false;
        }
        if (vrt_has_stream_id(&a->header) && vrt_has_stream_id(&b->header)) {
            if (a->fields.stream_id < b->fields.stream_id) {
                return true;
            }
            if (a->fields.stream_id > b->fields.stream_id) {
                return false;
            }
        }

        // Equality
        return false;
    }
};

/**
 * Packet -> File map.
 * std::map seems to be a better choice than std::unordered_map for few keys.
 * Static so it can be used in the signal handler.
 */
static std::map<PacketPtr, OutputFilePtr, ComparatorId> files_out;

/**
 * Handle shutdown signals gracefully by removing any temporary and output files before shutdown.
 *
 * \param signum Signal number.
 */
[[noreturn]] static void signal_handler(int signum) {
    for (auto& el : files_out) {
        try {
            el.second->remove_temporary();
        } catch (const fs::filesystem_error&) {
            // Do nothing. Just keep removing files.
        }
        try {
            el.second->remove_new();
        } catch (const fs::filesystem_error&) {
            // Do nothing. Just keep removing files.
        }
    }

    std::exit(signum);
}

/**
 * Describe differences between packets, as booleans.
 */
struct PacketDiffs {
    bool any_has_class_id{false};   // If any packet has Class ID
    bool any_has_stream_id{false};  // If any packet has Stream ID
    bool diff_oui{false};           // If OUI differs
    bool diff_icc{false};           // If Information class code differs
    bool diff_pcc{false};           // If Packet class code differs
    bool diff_sid{false};           // If Stream ID differs
};

/**
 * Get differences between packets.
 *
 * \return bools describing packet differences.
 */
static PacketDiffs packet_differences() {
    // Pointers to previous packets
    vrt_packet* prev_p{nullptr};    // Previous packet
    vrt_packet* prev_cid{nullptr};  // Previous packet with Class ID
    vrt_packet* prev_sid{nullptr};  // Previous packet with Stream ID

    // Calculate
    PacketDiffs ret;
    for (const auto& el : files_out) {
        if (prev_p != nullptr) {
            if (el.first->header.has.class_id) {
                ret.any_has_class_id = true;
            }
            if (vrt_has_stream_id(&el.first->header)) {
                ret.any_has_stream_id = true;
            }
            if (prev_cid != nullptr && el.first->header.has.class_id) {
                if (el.first->fields.class_id.oui != prev_cid->fields.class_id.oui) {
                    ret.diff_oui = true;
                }
                if (el.first->fields.class_id.information_class_code !=
                    prev_cid->fields.class_id.information_class_code) {
                    ret.diff_icc = true;
                }
                if (el.first->fields.class_id.packet_class_code != prev_cid->fields.class_id.packet_class_code) {
                    ret.diff_pcc = true;
                }
            }
            if (prev_sid != nullptr && vrt_has_stream_id(&el.first->header)) {
                if (el.first->fields.stream_id != prev_sid->fields.stream_id) {
                    ret.diff_sid = true;
                }
            }
        }

        // Save pointers to previous packets
        prev_p = el.first.get();
        if (el.first->header.has.class_id) {
            prev_cid = el.first.get();
        }
        if (vrt_has_stream_id(&el.first->header)) {
            prev_sid = el.first.get();
        }
    }

    return ret;
}

/**
 * Generate final output file path.
 *
 * \param file_path_in  Input file path.
 * \param packet        Input packet.
 * \param diffs         Packet differences.
 * \return Final output file path.
 */
static fs::path final_file_path(const fs::path& file_path_in, vrt_packet* packet, const PacketDiffs& diffs) {
    std::stringstream body{};
    if (diffs.any_has_class_id) {
        if (packet->header.has.class_id) {
            if (diffs.diff_oui) {
                body << '_' << std::hex << std::uppercase << packet->fields.class_id.oui;
            }
            if (diffs.diff_icc) {
                body << '_' << std::hex << std::uppercase << packet->fields.class_id.information_class_code;
            }
            if (diffs.diff_pcc) {
                body << '_' << std::hex << std::uppercase << packet->fields.class_id.packet_class_code;
            }
        } else {
            body << "_X_X_X";
        }
    }
    if (diffs.any_has_stream_id) {
        body << '_';
        if (vrt_has_stream_id(&packet->header)) {
            if (diffs.diff_sid) {
                body << std::hex << std::uppercase << packet->fields.stream_id;
            }
        } else {
            body << "X";
        }
    }

    // Separate path into parts
    fs::path p_in(file_path_in);
    fs::path dir{p_in.parent_path()};
    fs::path stem{p_in.stem()};
    fs::path ext{p_in.extension()};

    // Generate
    fs::path file_out{dir};
    file_out /= stem;
    file_out += body.str();
    file_out += ext;

    return file_out;
}

/**
 * Called when finishing writing, for renaming temporary files to final.
 *
 * \param file_path_in Input file path.
 *
 * \throw std::filesystem::filesystem_error On renaming error.
 */
static void finish(const std::string& file_path_in) {
    // Check if all Class and Stream IDs are the same
    if (files_out.size() <= 1) {
        std::cerr << "Warning: All packets have the same Class and Stream ID (if any). Use the existing '"
                  << file_path_in << "'." << std::endl;
        return;
    }

    try {
        PacketDiffs packet_diffs{packet_differences()};

        for (auto& el : files_out) {
            fs::path file_out{final_file_path(file_path_in, el.first.get(), packet_diffs)};
            el.second->rename(file_out);
        }
    } catch (...) {
        // Remove any newly created files before rethrow
        for (auto& el : files_out) {
            try {
                el.second->remove_new();
            } catch (const fs::filesystem_error&) {
                // Do nothing. Just keep removing.
            }
        }
        throw;
    }
}

/**
 * Process file contents.
 *
 * \param args Program arguments.
 *
 * \throw std::runtime_error If there's an error.
 */
void process(const ProgramArguments& args) {
    // Catch signals that aren't programming errors
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // Preallocate so it has room for header
    std::vector<uint32_t> buf(VRT_WORDS_HEADER);

    // Preallocate room for, perhaps byte swapped, header and fields
    std::array<uint32_t, VRT_WORDS_HEADER + VRT_WORDS_MAX_FIELDS> buf_header_fields{};

    // Note that stream is closed implicitly at destruction
    std::ifstream file_in(args.file_path_in, std::ios::in | std::ios::binary);
    if (file_in.fail()) {
        std::stringstream ss;
        ss << "Failed to open file '" << args.file_path_in << "'";
        throw std::runtime_error(ss.str());
    }

    // Clear, since it's static
    files_out.clear();

    // Go over all packets in input file
    for (int i{0};; ++i) {
        // No need to increase buffer size here, since buf preallocated VRT_SIZE_HEADER words and never shrinks
        file_in.read(reinterpret_cast<char*>(buf.data()), sizeof(uint32_t) * VRT_WORDS_HEADER);
        if (file_in.gcount() != 0 && file_in.gcount() != sizeof(uint32_t) * VRT_WORDS_HEADER) {
            std::stringstream ss;
            ss << "Packet #" << i << ": Failed to read header";
            throw std::runtime_error(ss.str());
        }
        if (file_in.eof()) {
            break;
        }

        // Byte swap header section if necessary
        if (args.do_byte_swap) {
            buf_header_fields[0] = bswap_32(buf[0]);
        } else {
            buf_header_fields[0] = buf[0];
        }

        // Parse header
        PacketPtr packet{std::make_shared<vrt_packet>()};
        int32_t   words_header{
            vrt_read_header(buf_header_fields.data(), buf_header_fields.size(), &packet->header, true)};
        if (words_header < 0) {
            std::stringstream ss;
            ss << "Packet #" << i << ": Failed to read header: " << vrt_string_error(words_header);
            throw std::runtime_error(ss.str());
        }

        // Enlarge read buffer for the next section if needed
        if (buf.size() < packet->header.packet_size) {
            buf.resize(packet->header.packet_size);
        }

        file_in.read(reinterpret_cast<char*>(buf.data() + words_header),
                     sizeof(uint32_t) * (packet->header.packet_size - words_header));
        // Do not handle EOF here
        if (file_in.gcount() != sizeof(uint32_t) * (packet->header.packet_size - words_header)) {
            std::stringstream ss;
            ss << "Packet #" << i << ": Failed to read remainder of packet";
            throw std::runtime_error(ss.str());
        }

        // Byte swap fields section if necessary
        int32_t words_fields = vrt_words_fields(&packet->header);
        if (args.do_byte_swap) {
            for (int j{1}; j < words_fields + 1; ++j) {
                buf_header_fields[j] = bswap_32(buf[j]);
            }
        } else {
            std::copy_n(buf.data() + 1, words_fields, buf_header_fields.data() + 1);
        }

        // Parse, print, and validate fields
        words_fields = vrt_read_fields(&packet->header, buf_header_fields.data() + words_header,
                                       buf_header_fields.size() - words_header, &packet->fields, true);
        if (words_fields < 0) {
            std::stringstream ss;
            ss << "Packet #" << i << ": Failed to read fields section: " << vrt_string_error(words_fields);
            throw std::runtime_error(ss.str());
        }

        // Find Class ID, Stream ID combination in map, or construct new output file if needed
        auto it{files_out.find(packet)};
        if (it == files_out.end()) {
            auto pair{files_out.emplace(packet, std::make_unique<OutputFile>(args.file_path_in, packet))};

            it = pair.first;
        }

        it->second->write(packet->header, &buf);
    }

    finish(args.file_path_in);
}

}  // namespace vrt::split
