#ifndef VRT_MERGE_OUTPUT_STREAM_H_
#define VRT_MERGE_OUTPUT_STREAM_H_

#include <fstream>
#include <sstream>

#include "output_stream.h"

namespace vrt::merge {

/**
 * Open output file for writing.
 *
 * \param file_path File path.
 *
 * \throw std::runtime_error If file fails to open.
 */
OutputStream::OutputStream(const std::string& file_path) {
    // Note that stream is closed implicitly at destruction
    file_out_.open(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (file_out_.fail()) {
        std::stringstream ss;
        ss << "Failed to open output file '" << file_path << "'";
        throw std::runtime_error(ss.str());
    }
}

}  // namespace vrt::merge

#endif
