#ifndef VRT_SOCKET_SRC_TIME_DIFFERENCE_H_
#define VRT_SOCKET_SRC_TIME_DIFFERENCE_H_

#include <chrono>
#include <cstdint>
#include <memory>
#include <ratio>

struct vrt_packet;

namespace vrt::socket {

/**
 * Calculate time difference in time between the first supplied packet and following.
 */
class TimeDifference {
   public:
    std::chrono::duration<int64_t, std::nano> calculate(const std::shared_ptr<vrt_packet>& pkt);

    /**
     * \return Sample rate [Hz].
     */
    double get_sample_rate() const { return sample_rate_; };

    /**
     * Set sample rate.
     *
     * \param sample_rate Sample rate [Hz].
     */
    void set_sample_rate(double sample_rate) { sample_rate_ = sample_rate; };

   private:
    std::chrono::duration<int64_t, std::nano> integer(const std::shared_ptr<vrt_packet>& pkt) const;
    std::chrono::duration<int64_t, std::nano> sample_count(const std::shared_ptr<vrt_packet>& pkt) const;
    std::chrono::duration<int64_t, std::nano> real_time(const std::shared_ptr<vrt_packet>& pkt) const;
    std::chrono::duration<int64_t, std::nano> free_running_count(const std::shared_ptr<vrt_packet>& pkt) const;

    bool is_nonnegative(const std::shared_ptr<vrt_packet>& pkt) const;

    /**
     * Number of picoseconds in a second.
     */
    static const uint64_t PS_IN_S{1000000000000};

    /**
     * Sample rate [Hz].
     */
    double sample_rate_{0.0};

    /**
     * First packet.
     */
    std::shared_ptr<vrt_packet> pkt_0_;

    /**
     * Timestamp in first packet.
     */
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> t_pkt_0_;
};

}  // namespace vrt::socket

#endif
