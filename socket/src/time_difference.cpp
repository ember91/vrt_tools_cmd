#include "time_difference.h"

#include <cmath>

#include <vrt/vrt_types.h>

namespace vrt::socket {

// For convenience
using PacketPtr = std::shared_ptr<vrt_packet>;
namespace tm    = ::std::chrono;
using Duration  = tm::duration<int64_t, std::nano>;

Duration TimeDifference::integer(const PacketPtr& pkt) const {
    return tm::seconds(pkt->fields.integer_seconds_timestamp - pkt_0->fields.integer_seconds_timestamp);
}

Duration TimeDifference::sample_count(const PacketPtr& pkt) const {
    Duration dur;
    if (sample_rate_ == 0.0) {
        // Sending at integer seconds is an OK strategy
        // TODO: Inform that supplying sample rate would give better results
        dur = tm::seconds(pkt->fields.integer_seconds_timestamp - pkt_0->fields.integer_seconds_timestamp);
    } else {
        // Due to validation we already know that fractional seconds is inside sample rate range.
        // Double precision is very likely sufficient precision.
        if (pkt->fields.fractional_seconds_timestamp >= pkt_0->fields.fractional_seconds_timestamp) {
            double frac{static_cast<double>(pkt->fields.fractional_seconds_timestamp -
                                            pkt_0->fields.fractional_seconds_timestamp) /
                        sample_rate_};
            dur = tm::seconds(pkt->fields.integer_seconds_timestamp - pkt_0->fields.integer_seconds_timestamp) +
                  tm::nanoseconds(static_cast<uint64_t>(frac * 1e9));
        } else {
            double frac{static_cast<double>(PS_IN_S - pkt_0->fields.fractional_seconds_timestamp +
                                            pkt->fields.fractional_seconds_timestamp) /
                        sample_rate_};
            dur = tm::seconds(pkt->fields.integer_seconds_timestamp - pkt_0->fields.integer_seconds_timestamp - 1) +
                  tm::nanoseconds(static_cast<uint64_t>(frac * 1e9));
        }
    }

    return dur;
}

Duration TimeDifference::real_time(const PacketPtr& pkt) const {
    return tm::seconds(pkt->fields.integer_seconds_timestamp) +
           tm::nanoseconds(pkt->fields.fractional_seconds_timestamp / 1000);
}

Duration TimeDifference::free_running_count(const PacketPtr& pkt) const {
    Duration dur;
    if (sample_rate_ == 0.0) {
        // Sending at integer seconds is an OK strategy
        // TODO: Inform that supplying sample rate would give better results
        dur = tm::seconds(pkt->fields.integer_seconds_timestamp - pkt_0->fields.integer_seconds_timestamp);
    } else {
        // Note that this works even in case of wraparound
        double sec{
            static_cast<double>(pkt->fields.fractional_seconds_timestamp - pkt_0->fields.integer_seconds_timestamp) /
            sample_rate_};
        uint64_t intg{static_cast<uint64_t>(sec)};  // Integer seconds
        double   frac{sec - std::floor(sec)};       // Fractional part

        dur = tm::seconds(intg) + tm::nanoseconds(static_cast<uint64_t>(frac * 1e9));
    }

    return dur;
}

/**
 * Calculate time between first packet and the current packet.
 *
 * \param pkt Current packet.
 *
 * \return Time to sleep.
 */
Duration TimeDifference::calculate(const PacketPtr& pkt) {
    bool is_first_pkt{pkt_0.get() == nullptr};

    // Save first packet
    if (is_first_pkt) {
        pkt_0 = pkt;
    }

    // TODO: Parse sample rate in packet

    // No need to use picosecond precision. Nanosecond resolution for sleeping is sufficient.

    // TODO: If timestamp is before, send instantly and display error

    // TODO: Check that first packet has the same TSI and TSF as following

    tm::duration<int64_t, std::nano> dur;
    if (pkt->header.tsi == VRT_TSI_NONE) {
        if (pkt->header.tsf == VRT_TSF_FREE_RUNNING_COUNT) {
            dur = free_running_count(pkt);
        } else {
            // We don't know better. Send instantly.
            dur = tm::seconds(0);
        }
    } else {
        switch (pkt->header.tsf) {
            case VRT_TSF_NONE: {
                dur = integer(pkt);
                break;
            }
            case VRT_TSF_SAMPLE_COUNT: {
                dur = sample_count(pkt);
                break;
            }
            case VRT_TSF_REAL_TIME: {
                dur = real_time(pkt);
                break;
            }
            case VRT_TSF_FREE_RUNNING_COUNT: {
                // Just ignore integer seconds...
                dur = free_running_count(pkt);
                break;
            }
        }
    }

    tm::time_point<tm::system_clock, tm::nanoseconds> t_pkt_now{tm::time_point<tm::system_clock, tm::nanoseconds>(dur)};
    tm::time_point<tm::system_clock, tm::nanoseconds> t_loc_now{tm::system_clock::now()};

    // Save first time points
    if (is_first_pkt) {
        t_pkt_0 = t_pkt_now;
        t_loc_0 = t_loc_now;
    }

    return (t_pkt_now - t_pkt_0) - (t_loc_now - t_loc_0);
}

}  // namespace vrt::socket
