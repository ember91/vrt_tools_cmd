#ifndef LIB_COMMON_INCLUDE_COMMON_INPUT_STREAM_H_
#define LIB_COMMON_INCLUDE_COMMON_INPUT_STREAM_H_

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

struct vrt_packet;

namespace vrt::common {

/**
 * Input stream.
 */
class InputStream {
   public:
    InputStream(std::filesystem::path file_path, bool do_byte_swap, bool do_validate = true);

    bool read_next_packet();
    bool skip_next_packet();
    void reset();

    /**
     * \return Output file path.
     */
    const std::filesystem::path& get_file_path() const { return file_path_; }

    /**
     * \return Last read packet.
     */
    std::shared_ptr<vrt_packet> get_packet() const { return packet_; }

    /**
     * \return Input file size [B].
     */
    std::streampos get_file_size() const { return file_size_bytes_; }

    /**
     * \return Non-byte swapped packet buffer.
     */
    const std::vector<uint32_t>& get_buffer() { return buf_; }

    /**
     * \return Byte swapped packet buffer.
     */
    const std::vector<uint32_t>& get_buffer_byte_swap() { return buf_; }

   private:
    bool read_parse_header();

    const std::filesystem::path file_path_;
    const bool                  do_byte_swap_;
    const bool                  do_validate_;

    std::shared_ptr<vrt_packet> packet_;
    std::ifstream               file_;
    std::streampos              file_size_bytes_{0};
    std::vector<uint32_t>       buf_;
    std::vector<uint32_t>       buf_byte_swap_;
    uint64_t                    pkt_idx_{0};
};

}  // namespace vrt::common

#endif
