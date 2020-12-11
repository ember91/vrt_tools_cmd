
#include "output_stream_rename.h"

#include <filesystem>
#include <fstream>
#include <utility>

#include "output_stream.h"

namespace vrt::split {

namespace fs = ::std::filesystem;

/**
 * Open output file for writing.
 *
 * \param file_path File path.
 *
 * \throw std::runtime_error If file fails to open.
 */
OutputStreamRename::OutputStreamRename(std::filesystem::path file_path) : OutputStream(file_path) {}

/**
 * Destructor. Remove any temporary file.
 */
OutputStreamRename::~OutputStreamRename() {
    // Ensure removed at destruction only if final name differ
    if (file_path_ != file_path_renamed_) {
        // Explicitly call parent function here
        OutputStream::remove_file();
    }
}

/**
 * Close and remove file. Do not write after this.
 */
void OutputStreamRename::remove_file() {
    OutputStream::remove_file();
    try {
        fs::remove(file_path_renamed_);
    } catch (const fs::filesystem_error&) {
        // Do nothing
    }
}

/**
 * Close and rename file. Writing will not work anymore.
 *
 * \param path File path to new file.
 *
 * \throw std::filesystem::filesystem_error On I/O or renaming error.
 */
void OutputStreamRename::rename_file(fs::path path) {
    file_path_renamed_ = std::move(path);
    try {
        file_.close();
    } catch (const std::ios::failure&) {
        // Do nothing
    }
    fs::rename(file_path_, file_path_renamed_);
}

}  // namespace vrt::split
