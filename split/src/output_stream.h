#ifndef VRT_SPLIT_SRC_OUTPUT_STREAM_H_
#define VRT_SPLIT_SRC_OUTPUT_STREAM_H_

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <vrt/vrt_types.h>

namespace vrt::split {

/**
 * Output file.
 */
class OutputStream {
   public:
    OutputStream(const std::filesystem::path& file_path_in, std::shared_ptr<vrt_packet> packet);
    virtual ~OutputStream();

    std::filesystem::path generate_temporary_file_path(const std::filesystem::path& file_path_in) const;

    void rename(const std::filesystem::path& p);

    void remove_temporary() const;
    void remove_new() const;

    std::ofstream& get_file() { return file_; }

   private:
    const std::shared_ptr<vrt_packet> packet_;

    std::filesystem::path file_path_tmp_;
    std::filesystem::path file_path_new_;
    std::ofstream         file_;
};

}  // namespace vrt::split

#endif
