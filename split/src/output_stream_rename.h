#ifndef VRT_SPLIT_SRC_OUTPUT_STREAM_H_
#define VRT_SPLIT_SRC_OUTPUT_STREAM_H_

#include <filesystem>
#include <fstream>

#include "output_stream.h"

namespace vrt::split {

/**
 * Output stream that can be renamed.
 */
class OutputStreamRename : public OutputStream {
   public:
    OutputStreamRename(std::filesystem::path file_path);
    virtual ~OutputStreamRename();

    virtual void remove_file() override;

    void rename_file(std::filesystem::path path);

   private:
    std::filesystem::path file_path_renamed_;
};

}  // namespace vrt::split

#endif
