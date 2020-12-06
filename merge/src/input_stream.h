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

    void write(std::ofstream& of);

    /**
     * Get last read packet header.
     *
     * \return Packet header.
     */
    const vrt_header& get_header() const { return header_; }

    /**
     * Get last read fields section.
     *
     * \return Fields section.
     */
    const vrt_fields& get_fields() const { return fields_; }

    /**
     * Get current packet index in file.
     *
     * \return Packet index.
     */
    uint64_t get_packet_index() const { return pkt_idx_; }

    /**
     * Get output file path.
     *
     * \return Output file path.
     */
    std::filesystem::path get_file_path() const { return file_path_; }
    /**
     * Get file size.
     *
     * \return File size [B].
     */
    std::streampos const get_file_size() { return file_size_bytes_; }

   private:
    const std::filesystem::path file_path_;
    const bool                  do_byte_swap_;

    vrt_header header_;
    vrt_fields fields_;

    std::streampos        file_size_bytes_;
    std::ifstream         file_;
    uint64_t              pkt_idx_{0};
    std::vector<uint32_t> buf_;
};

}  // namespace vrt::merge

#endif
