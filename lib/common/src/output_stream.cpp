#include "common/output_stream.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace vrt::common {

namespace fs = ::std::filesystem;

/**
 * Open output file for writing.
 *
 * \param file_path File path.
 *
 * \throw std::runtime_error If file fails to open.
 */
OutputStream::OutputStream(fs::path file_path) : file_path_{std::move(file_path)} {
    file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);

    // Open file for non-appended writing
    try {
        file_.open(file_path_, std::ios::out | std::ios::binary | std::ios::trunc);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to open output file " << file_path_;
        throw std::runtime_error(ss.str());
    }
}

/**
 * Destructor.
 */
OutputStream::~OutputStream() {}

/**
 * Close and remove file. Do not write after this.
 */
void OutputStream::remove_file() {
    try {
        file_.close();
    } catch (const std::ios::failure&) {
        // Do nothing
    }
    try {
        fs::remove(file_path_);
    } catch (const fs::filesystem_error&) {
        // Do nothing
    }
}

/**
 * Write buffer to file.
 *
 * \param buf   Buffer to write.
 * \param words Number of words to write.
 *
 * \throw std::runtime_error On I/O error.
 */
void OutputStream::write(const std::vector<uint32_t>& buf, int32_t words) {
    try {
        file_.write(reinterpret_cast<const char*>(buf.data()), sizeof(uint32_t) * words);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to write to output file";
        throw std::runtime_error(ss.str());
    }
}

}  // namespace vrt::common
