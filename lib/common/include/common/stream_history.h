#ifndef VRT_COMMON_SRC_STREAM_HISTORY_H_
#define VRT_COMMON_SRC_STREAM_HISTORY_H_

#include <cstdint>
#include <memory>

struct vrt_packet;

namespace vrt::common {

/**
 * History of a stream.
 */
class StreamHistory {
   public:
    explicit StreamHistory(double sample_rate) : sample_rate_{sample_rate} {}

    void update(const std::shared_ptr<vrt_packet>& packet);

    std::shared_ptr<vrt_packet> get_packet_first() const { return packet_first_; }
    std::shared_ptr<vrt_packet> get_packet_current() const { return packet_cur_; }

    uint64_t get_number_of_packets() const { return n_packets_; }

    double get_sample_rate() const { return sample_rate_; }

   private:
    std::shared_ptr<vrt_packet> packet_first_;
    std::shared_ptr<vrt_packet> packet_cur_;
    double                      sample_rate_{0}; /**< Sample rate in last IF context packet. */

    uint64_t n_packets_{0};
};

}  // namespace vrt::common

#endif
