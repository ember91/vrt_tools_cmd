
#include "output_stream.h"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

namespace vrt::split {

namespace fs = std::filesystem;

/**
 * Constructor. Open file for writing.
 *
 * \param file_path_in Input file path.
 * \param packet       Packet header and fields.
 *
 * \throw std::runtime_error On temporary file creation error.
 */
OutputStream::OutputStream(const std::filesystem::path& file_path_in, std::shared_ptr<vrt_packet> packet)
    : packet_{std::move(packet)} {
    file_path_tmp_ = generate_temporary_file_path(file_path_in);

    file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);

    // Open file for non-appended writing
    try {
        file_.open(file_path_tmp_, std::ios::out | std::ios::binary | std::ios::trunc);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to open temporary output file '" << file_path_tmp_ << "'";
        std::runtime_error(ss.str());
    }
}

/**
 * Destructor. Remove any temporary file.
 */
OutputStream::~OutputStream() {
    try {
        remove_temporary();
    } catch (const fs::filesystem_error&) {
        // Ignore error
    }
}

/**
 * Generate a temporary and for this application unique file path.
 *
 * \param file_path_in Input file path.
 * \return Temporary file path.
 */
fs::path OutputStream::generate_temporary_file_path(const fs::path& file_path_in) const {
    // Separate path into parts
    fs::path dir{file_path_in.parent_path()};
    fs::path stem{file_path_in.stem()};
    fs::path ext{file_path_in.extension()};

    // Create filename as dir/stem_{Has Class ID}_{OUI}_{ICC}_{PCC}_{Has Stream ID}_{Stream ID}.{Extension}
    fs::path file_path_tmp{dir};
    file_path_tmp /= stem;
    std::stringstream ss;
    ss << '_' << (packet_->header.has.class_id ? '1' : '0');
    ss << '_' << std::hex << std::uppercase << packet_->fields.class_id.oui << "_"
       << packet_->fields.class_id.information_class_code << "_" << packet_->fields.class_id.packet_class_code;
    file_path_tmp += ss.str();
    ss.str("");
    ss << '_' << (vrt_has_stream_id(&packet_->header) ? '1' : '0');
    ss << '_' << std::hex << std::uppercase << packet_->fields.stream_id;
    file_path_tmp += ss.str();
    file_path_tmp += ext;

    return file_path_tmp;
}

/**
 * Write to temporary file.
 *
 * \param header Header to write.
 * \param buf    Buffer with data to write.
 *
 * \throw std::runtime_error On write failure.
 */
void OutputStream::write(const vrt_header& header, const std::vector<uint32_t>& buf) {
    try {
        file_.write(reinterpret_cast<const char*>(buf.data()), sizeof(uint32_t) * header.packet_size);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to write to file '" << file_path_tmp_ << "'";
        throw std::runtime_error(ss.str());
    }
}

/**
 * Make temporary file the output file.
 *
 * \param p File path to new file.
 *
 * \throw std::filesystem::filesystem_error On I/O or renaming error.
 */
void OutputStream::rename(const fs::path& p) {
    file_.close();
    fs::rename(file_path_tmp_, p);
    file_path_new_ = p;
}

/**
 * Remove temporary file, if any.
 *
 * \throw std::filesystem::filesystem_error On I/O error.
 */
void OutputStream::remove_temporary() const {
    if (!file_path_tmp_.empty()) {
        fs::remove(file_path_tmp_);
    }
}

/**
 * Remove output file, if any.
 *
 * \throw std::filesystem::filesystem_error On I/O error.
 */
void OutputStream::remove_new() const {
    if (!file_path_new_.empty()) {
        fs::remove(file_path_new_);
    }
}

}  // namespace vrt::split
