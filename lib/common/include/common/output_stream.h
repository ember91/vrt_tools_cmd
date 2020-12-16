#ifndef LIB_COMMON_INCLUDE_COMMON_OUTPUT_STREAM_H_
#define LIB_COMMON_INCLUDE_COMMON_OUTPUT_STREAM_H_

#include <filesystem>
#include <fstream>
#include <vector>

namespace vrt::common {

/**
 * Output stream.
 */
class OutputStream {
   public:
    OutputStream(std::filesystem::path file_path);
    virtual ~OutputStream();

    virtual void remove_file();

    void write(const std::vector<uint32_t>& buf, int32_t words);

   protected:
    const std::filesystem::path file_path_;

    std::ofstream file_;
};

}  // namespace vrt::common

#endif
