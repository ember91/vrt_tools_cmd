#ifndef VRT_MERGE_OUTPUT_STREAM_H_
#define VRT_MERGE_OUTPUT_STREAM_H_

#include <filesystem>
#include <fstream>
#include <sstream>

#include "output_stream.h"

namespace fs = std::filesystem;

namespace vrt::merge {

/**
 * Open output file for writing.
 *
 * \param file_path File path.
 *
 * \throw std::runtime_error If file fails to open.
 */
OutputStream::OutputStream(std::string file_path) : file_path_{std::move(file_path)} {
    file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);

    // Open file for reading
    try {
        file_.open(file_path_, std::ios::out | std::ios::binary | std::ios::trunc);
    } catch (const std::ios::failure&) {
        std::stringstream ss;
        ss << "Failed to open output file '" << file_path_ << "'";
        throw std::runtime_error(ss.str());
    }
}

/**
 * Close and remove file. Do not write after this.
 */
void OutputStream::remove_file() {
    try {
        file_.close();
    } catch (const fs::filesystem_error&) {
        // Do nothing
    }
    try {
        fs::remove(file_path_);
    } catch (const fs::filesystem_error&) {
        // Do nothing
    }
}

}  // namespace vrt::merge

#endif
