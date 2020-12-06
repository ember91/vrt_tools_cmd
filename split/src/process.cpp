#include "process.h"

#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

#include "Progress-CPP/ProgressBar.hpp"
#include "comparator_id.h"
#include "input_stream.h"
#include "output_stream.h"
#include "program_arguments.h"

namespace fs = std::filesystem;

namespace vrt::split {

// For convenience
using PacketPtr       = std::shared_ptr<vrt_packet>;
using OutputStreamPtr = std::unique_ptr<OutputStream>;

/**
 * Packet -> File map.
 * std::map seems to be a better choice than std::unordered_map for few keys.
 * Static so it can be used in the signal handler.
 */
static std::map<PacketPtr, OutputStreamPtr, ComparatorId> output_streams;

/**
 * Handle shutdown signals gracefully by removing any temporary and output files before shutdown.
 *
 * \param signum Signal number.
 */
[[noreturn]] static void signal_handler(int signum) {
    for (auto& el : output_streams) {
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
 * \return booleans describing packet differences.
 */
static PacketDiffs packet_differences() {
    // Pointers to previous packets
    vrt_packet* prev_p{nullptr};    // Previous packet
    vrt_packet* prev_cid{nullptr};  // Previous packet with Class ID
    vrt_packet* prev_sid{nullptr};  // Previous packet with Stream ID

    // Calculate
    PacketDiffs ret;
    for (const auto& el : output_streams) {
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
    if (output_streams.size() <= 1) {
        for (auto& el : output_streams) {
            try {
                el.second->remove_temporary();
            } catch (const fs::filesystem_error&) {
                // Do nothing. Just keep removing.
            }
        }
        std::cerr << "Warning: All packets have the same Class and Stream ID (if any). Use the existing '"
                  << file_path_in << "'." << std::endl;
        return;
    }

    try {
        PacketDiffs packet_diffs{packet_differences()};

        for (auto& el : output_streams) {
            fs::path file_out{final_file_path(file_path_in, el.first.get(), packet_diffs)};
            el.second->rename(file_out);
        }
    } catch (...) {
        // Remove any newly created files before rethrow
        for (auto& el : output_streams) {
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

    InputStream input_stream(args.file_path_in, args.do_byte_swap);

    // Clear, since it's static
    output_streams.clear();

    // Progress bar
    progresscpp::ProgressBar progress(static_cast<size_t>(input_stream.get_file_size()), 70);

    // Go over all packets in input file
    for (int i{0};; ++i) {
        if (!input_stream.read_next()) {
            break;
        }

        // Find Class ID, Stream ID combination in map, or construct new output file if needed
        PacketPtr packet{input_stream.get_packet()};
        auto      it{output_streams.find(packet)};
        if (it == output_streams.end()) {
            auto pair{output_streams.emplace(packet, std::make_unique<OutputStream>(args.file_path_in, packet))};

            it = pair.first;
        }

        it->second->write(packet->header, input_stream.get_data_buffer());

        // Handle progress bar
        progress += sizeof(uint32_t) * packet->header.packet_size;
        if (progress.get_ticks() % 65536 == 0) {
            progress.display();
        }
    }

    progress.done();

    finish(args.file_path_in);
}

}  // namespace vrt::split
