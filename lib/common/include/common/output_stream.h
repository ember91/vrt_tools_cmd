#ifndef VRT_MERGE_SRC_OUTPUT_STREAM_H_
#define VRT_MERGE_SRC_OUTPUT_STREAM_H_

#include <filesystem>
#include <fstream>

namespace vrt {

/**
 * Output stream.
 */
class OutputStream {
   public:
    OutputStream(std::filesystem::path file_path);
    virtual ~OutputStream();

    virtual void remove_file();

    /**
     * Get reference to file.
     *
     * \return Output file.
     */
    std::ofstream& get_file() { return file_; }

   protected:
    const std::filesystem::path file_path_;

    std::ofstream file_;
};

}  // namespace vrt

#endif
