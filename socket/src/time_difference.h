#ifndef VRT_SOCKET_SRC_TIME_DIFFERENCE_H_
#define VRT_SOCKET_SRC_TIME_DIFFERENCE_H_

#include <chrono>
#include <memory>

#include <vrt/vrt_types.h>

namespace vrt::socket {

class TimeDifference {
   public:
    /**
     * Constructor.
     *
     * \param sample_rate Supplied sample rate
     */
    TimeDifference(double sample_rate = 0.0) : sample_rate_{sample_rate} {}

    std::chrono::duration<int64_t, std::nano> calculate(const std::shared_ptr<vrt_packet>& pkt);

   private:
    std::chrono::duration<int64_t, std::nano> integer(const std::shared_ptr<vrt_packet>& pkt) const;
    std::chrono::duration<int64_t, std::nano> sample_count(const std::shared_ptr<vrt_packet>& pkt) const;
    std::chrono::duration<int64_t, std::nano> real_time(const std::shared_ptr<vrt_packet>& pkt) const;
    std::chrono::duration<int64_t, std::nano> free_running_count(const std::shared_ptr<vrt_packet>& pkt) const;

    /**
     * Number of picoseconds in a second.
     */
    static const uint64_t PS_IN_S{1000000000000};

    /**
     * Sample rate [Hz].
     */
    const double sample_rate_;

    /**
     * First packet.
     */
    std::shared_ptr<vrt_packet> pkt_0;

    /**
     * Timestamp in first packet.
     */
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> t_pkt_0;

    /**
     * Time when first packet was parsed.
     */
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> t_loc_0;
};

}  // namespace vrt::socket

#endif
