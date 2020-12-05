#ifndef VRT_MERGE_INPUT_STREAM_H_
#define VRT_MERGE_INPUT_STREAM_H_

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <vector>

#include <vrt/vrt_types.h>

namespace vrt::merge {

/**
 * Input stream.
 */
class InputStream {
   public:
    InputStream(std::filesystem::path file_path, bool byte_swap);

    bool read_next_packet();

    const vrt_header&     get_header() const { return header_; }
    const vrt_fields&     get_fields() const { return fields_; }
    uint64_t              get_packet_index() const { return pkt_idx_; }
    std::filesystem::path get_file_path() const { return file_path_; }

    void write(std::ofstream& of);

   private:
    const std::filesystem::path file_path_;
    const bool                  do_byte_swap_;

    vrt_header header_;
    vrt_fields fields_;

    std::ifstream         file_in_;
    uint64_t              pkt_idx_;
    std::vector<uint32_t> buf_;
};

}  // namespace vrt::merge

#endif
