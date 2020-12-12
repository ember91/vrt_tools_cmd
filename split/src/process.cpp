#include "process.h"

#include <cstdint>
#include <cstdio>
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
#include "output_stream_rename.h"
#include "packet_id_differences.h"
#include "program_arguments.h"

namespace vrt::split {

namespace fs = ::std::filesystem;

// For convenience
using PacketPtr             = std::shared_ptr<vrt_packet>;
using OutputStreamRenamePtr = std::unique_ptr<OutputStreamRename>;
using PacketOutputStreamMap = std::map<PacketPtr, OutputStreamRenamePtr, ComparatorId>;

/**
 * Generate a temporary and for this application unique file path.
 *
 * \param file_path_in Input file path.
 * \param packet       Packet.
 *
 * \return Temporary file path.
 */
static fs::path generate_temporary_file_path(const fs::path& file_path_in, const vrt_packet& packet) {
    // Separate path into parts
    fs::path dir{file_path_in.parent_path()};
    fs::path stem{file_path_in.stem()};
    fs::path ext{file_path_in.extension()};

    // Create filename as dir/stem_{Has Class ID}_{OUI}_{ICC}_{PCC}_{Has Stream ID}_{Stream ID}.{Extension}
    fs::path file_path_tmp{dir};
    file_path_tmp /= stem;
    std::stringstream ss;
    ss << '_' << (packet.header.has.class_id ? '1' : '0');
    ss << '_' << std::hex << std::uppercase << packet.fields.class_id.oui << "_"
       << packet.fields.class_id.information_class_code << "_" << packet.fields.class_id.packet_class_code;
    file_path_tmp += ss.str();
    ss.str("");
    ss << '_' << (vrt_has_stream_id(&packet.header) ? '1' : '0');
    ss << '_' << std::hex << std::uppercase << packet.fields.stream_id;
    file_path_tmp += ss.str();
    file_path_tmp += ext;

    return file_path_tmp;
}

/**
 * Generate final output file path.
 *
 * \param file_path_in  Input file path.
 * \param packet        Input packet.
 * \param diffs         Packet differences.
 * \return Final output file path.
 */
static fs::path final_file_path(const fs::path& file_path_in, vrt_packet* packet, const PacketIdDiffs& diffs) {
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
 * \param file_path_in   Input file path.
 * \param output_streams Output streams.
 *
 * \throw std::filesystem::filesystem_error On renaming error.
 */
static void finish(const fs::path& file_path_in, const PacketOutputStreamMap& output_streams) {
    // Check if all Class and Stream IDs are the same
    if (output_streams.size() <= 1) {
        for (auto& el : output_streams) {
            el.second->remove_file();
        }
        std::cerr << "Warning: All packets have the same Class and Stream ID (if any). Use the existing "
                  << file_path_in << '.' << std::endl;
        return;
    }

    try {
        // Make vector from map
        std::vector<PacketPtr> v;
        v.reserve(output_streams.size());
        for (const auto& el : output_streams) {
            v.push_back(el.first);
        }

        PacketIdDiffs packet_diffs{packet_id_differences(v)};

        for (auto& el : output_streams) {
            fs::path file_out{final_file_path(file_path_in, el.first.get(), packet_diffs)};
            el.second->rename_file(file_out);
        }
    } catch (...) {
        // Remove any newly created files before rethrow
        for (auto& el : output_streams) {
            el.second->remove_file();
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
    /**
     * Packet -> File map.
     */
    PacketOutputStreamMap output_streams;

    InputStream input_stream(args.file_path_in, args.do_byte_swap);

    // Progress bar
    progresscpp::ProgressBar progress(static_cast<size_t>(input_stream.get_file_size()), 70);

    // Go over all packets in input file
    for (int i{0};; ++i) {
        if (!input_stream.read_next_packet()) {
            break;
        }

        // Find Class ID, Stream ID combination in map, or construct new output file if needed
        PacketPtr packet{input_stream.get_packet()};
        auto      it{output_streams.find(packet)};
        if (it == output_streams.end()) {
            fs::path p{generate_temporary_file_path(args.file_path_in, *packet.get())};
            auto     pair{output_streams.emplace(packet, std::make_unique<OutputStreamRename>(p))};

            it = pair.first;
        }

        // Write input packet to output
        input_stream.write(it->second->get_file());

        // Handle progress bar
        progress += sizeof(uint32_t) * packet->header.packet_size;
        if (progress.get_ticks() % 65536 == 0) {
            progress.display();
        }
    }

    progress.done();

    finish(args.file_path_in, output_streams);
}

}  // namespace vrt::split
