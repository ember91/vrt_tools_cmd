#ifndef VRT_SPLIT_SRC_INPUT_STREAM_H_
#define VRT_SPLIT_SRC_INPUT_STREAM_H_

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <vrt/vrt_types.h>

namespace vrt::split {

/**
 * Input stream.
 */
class InputStream {
   public:
    InputStream(const std::filesystem::path& file_path, bool do_byte_swap);

    bool read_next();

    /**
     * Get last read packet.
     *
     * \return Packet.
     */
    std::shared_ptr<vrt_packet> get_packet() const { return packet_; }

    /**
     * Get data buffer.
     *
     * \return Data buffer.
     */
    const std::vector<uint32_t>& get_data_buffer() const { return buf_; }

    /**
     * Get input file size.
     *
     * \return File size [B].
     */
    std::streampos get_file_size() const { return file_size_bytes_; }

   private:
    const bool do_byte_swap_;

    std::shared_ptr<vrt_packet> packet_;
    std::ifstream               file_;
    std::streampos              file_size_bytes_{0};
    std::vector<uint32_t>       buf_;
    uint64_t                    pkt_idx_{0};
};

}  // namespace vrt::split

#endif
