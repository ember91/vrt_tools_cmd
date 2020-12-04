#ifndef VRT_SPLIT_SRC_OUTPUT_FILE_H_
#define VRT_SPLIT_SRC_OUTPUT_FILE_H_

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <vrt/vrt_types.h>

namespace vrt::split {

class OutputFile {
   public:
    OutputFile(const std::filesystem::path& file_path_in, std::shared_ptr<vrt_packet> packet);
    virtual ~OutputFile();

    std::filesystem::path generate_temporary_file_path(const std::filesystem::path& file_path_in);

    void write(const vrt_header& header, std::vector<uint32_t>* buf);
    void rename(const std::filesystem::path& p);

    void remove_temporary();
    void remove_new();

   private:
    std::shared_ptr<vrt_packet> packet_;
    std::filesystem::path       file_path_tmp_;
    std::filesystem::path       file_path_new_;
    std::ofstream               fp_;
};

}  // namespace vrt::split

#endif
