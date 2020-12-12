#include "printer.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>

#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

#include "packet_id_differences.h"
#include "stream_history.h"

namespace vrt::length {

/**
 * Number of picoseconds in a second.
 */
static const uint64_t PS_IN_S{1000000000000};

/**
 * Calculate difference in integer seconds.
 *
 * \param fir  First packet in stream.
 * \param cur  Current packet in stream.
 * \return Difference [s].
 */
static uint32_t d_int(vrt_packet* fir, vrt_packet* cur) {
    return cur->fields.integer_seconds_timestamp - fir->fields.integer_seconds_timestamp;
}

/**
 * Calculate difference in fractional seconds.
 *
 * \param fir  First packet in stream.
 * \param cur  Current packet in stream.
 * \return Difference.
 */
static uint64_t d_frac(vrt_packet* fir, vrt_packet* cur) {
    return cur->fields.fractional_seconds_timestamp - fir->fields.fractional_seconds_timestamp;
}

/**
 * Print Class and Stream ID information.
 *
 * \param p A packet in stream.
 */
static void print_ids(vrt_packet* p, const PacketIdDiffs& differences) {
    std::cout << std::hex << std::setfill('0');
    if (p->header.has.class_id) {
        if (differences.diff_oui || differences.diff_icc || differences.diff_pcc) {
            std::cout << "Class ID\n";
            if (differences.diff_oui) {
                std::cout << "  OUI: 0x" << std::setw(6) << p->fields.class_id.oui << '\n';
            }
            if (differences.diff_icc) {
                std::cout << "  Information class code: 0x" << std::setw(4) << p->fields.class_id.information_class_code
                          << '\n';
            }
            if (differences.diff_pcc) {
                std::cout << "  Packet class code: 0x" << std::setw(4) << p->fields.class_id.packet_class_code << '\n';
            }
        }
    } else if (differences.any_has_class_id) {
        std::cout << "Class ID: None\n";
    }
    if (vrt_has_stream_id(&p->header)) {
        if (differences.diff_sid) {
            std::cout << "Stream ID: 0x" << std::setw(8) << p->fields.stream_id << '\n';
        }
    } else if (differences.any_has_stream_id) {
        std::cout << "Stream ID: None\n";
    }

    // Reset stream manipulators
    std::cout << std::setw(0) << std::dec << std::setfill(' ');
}

/**
 * Print only integer time. Assumes it is set.
 *
 * \param fir  First packet in stream.
 * \param cur  Current packet in stream.
 */
static void print_integer_time(vrt_packet* fir, vrt_packet* cur) {
    if (fir->fields.integer_seconds_timestamp > cur->fields.integer_seconds_timestamp) {
        std::cerr << "Integer timestamp of first packet occurs after timestamp of last packet\n";
    } else {
        std::cout << "Packet time difference: " << d_int(fir, cur) << " s\n";
    }
}

/**
 * Print only fractional time. Assumes it is set.
 *
 * \param fir  First packet in stream.
 * \param cur  Current packet in stream.
 */
static void print_fractional_time(vrt_packet* fir, vrt_packet* cur) {
    // Note that it's hard to figure out anything about sample time or real time fractional timestamps
    if (cur->header.tsf == VRT_TSF_FREE_RUNNING_COUNT) {
        // Calculating this difference is probably OK. Even with high sample rates it works as long as the recording
        // isn't in the order of years.
        std::cout << d_frac(fir, cur) << " samples\n";
    }
}

/**
 * Print integer and fractional time for Real time.
 *
 * \param fir  First packet in stream.
 * \param cur  Current packet in stream.
 */
static void print_integer_fractional_real_time(vrt_packet* fir, vrt_packet* cur) {
    if (fir->fields.fractional_seconds_timestamp >= PS_IN_S || cur->fields.fractional_seconds_timestamp >= PS_IN_S) {
        std::cerr << "Fractional seconds outside bounds\n";
        std::cout << d_int(fir, cur) << " s\n";
    } else {
        if (cur->fields.fractional_seconds_timestamp > fir->fields.fractional_seconds_timestamp) {
            std::cout << d_int(fir, cur) << '.' << std::setfill('0') << std::setw(11) << d_frac(fir, cur) << " s\n";
        } else {
            std::cout << d_int(fir, cur) - 1 << '.' << std::setfill('0') << std::setw(11) << PS_IN_S - d_frac(cur, fir)
                      << " s\n";
        }

        // Reset stream manipulators
        std::cout << std::setw(0) << std::setfill(' ');
    }
}

/**
 * Print integer and fractional time for Free running count.
 *
 * \param fir  First packet in stream.
 * \param cur  Current packet in stream.
 */
static void print_integer_fractional_free_running_count(vrt_packet* fir, vrt_packet* cur) {
    std::cout << d_int(fir, cur) << " s\n";
    std::cout << d_frac(fir, cur) << " samples\n";
}

/**
 * Print integer and fractional time for Sample count.
 *
 * \param fir         First packet in stream.
 * \param cur         Current packet in stream.
 * \param sample_rate Sample rate [Hz].
 */
static void print_integer_fractional_sample_count(vrt_packet* fir, vrt_packet* cur, double sample_rate) {
    if (sample_rate == 0.0) {
        if (cur->fields.fractional_seconds_timestamp > fir->fields.fractional_seconds_timestamp) {
            std::cout << d_int(fir, cur) << " s\n";
            std::cout << d_frac(fir, cur) << " samples\n";
        } else {
            std::cout << d_int(fir, cur) << " s\n";
            std::cout << "Cannot print fractional second timestamps since sample rate is "
                         "unknown\n";
        }
    } else {
        if (fir->fields.fractional_seconds_timestamp >= sample_rate ||
            cur->fields.fractional_seconds_timestamp >= sample_rate) {
            std::cerr << "Fractional seconds outside bounds\n";
            std::cout << d_int(fir, cur) << " s\n";
        } else {
            if (cur->fields.fractional_seconds_timestamp > fir->fields.fractional_seconds_timestamp) {
                std::cout << d_int(fir, cur) << '.' << std::setfill('0') << std::setw(11)
                          << static_cast<uint64_t>((d_frac(fir, cur)) / sample_rate * PS_IN_S) << " s\n";
            } else {
                std::cout << d_int(fir, cur) - 1 << '.' << std::setfill('0') << std::setw(11)
                          << static_cast<uint64_t>((sample_rate - cur->fields.fractional_seconds_timestamp -
                                                    fir->fields.fractional_seconds_timestamp) /
                                                   sample_rate * PS_IN_S)
                          << " s\n";
            }
        }

        // Reset stream manipulators
        std::cout << std::setw(0) << std::setfill(' ');
    }
}

/**
 * Print both integer and fractional time.
 *
 * \param fir         First packet in stream.
 * \param cur         Current packet in stream.
 * \param sample_rate Sample rate [Hz].
 */
static void print_integer_fractional_time(vrt_packet* fir, vrt_packet* cur, double sample_rate) {
    if (cur->header.tsf != VRT_TSF_FREE_RUNNING_COUNT &&
        fir->fields.integer_seconds_timestamp == cur->fields.integer_seconds_timestamp &&
        fir->fields.fractional_seconds_timestamp > cur->fields.fractional_seconds_timestamp) {
        std::cerr << "Timestamp of first packet occurs after timestamp of last packet\n";
    } else {
        switch (cur->header.tsf) {
            case VRT_TSF_REAL_TIME: {
                print_integer_fractional_real_time(fir, cur);
                break;
            }
            case VRT_TSF_SAMPLE_COUNT: {
                print_integer_fractional_sample_count(fir, cur, sample_rate);
                break;
            }
            case VRT_TSF_FREE_RUNNING_COUNT: {
                print_integer_fractional_free_running_count(fir, cur);
                break;
            }
            default: {
                // Do nothing. Should never end up here.
                break;
            }
        }
    }
}

/**
 * Print differences between first and last packets in a specific stream.
 *
 * \param stream_history Stream history.
 * \param differences    Differences between streams.
 */
void print_difference(const StreamHistory& stream_history, const PacketIdDiffs& differences) {
    vrt_packet* fir{stream_history.get_packet_first().get()};
    vrt_packet* cur{stream_history.get_packet_current().get()};
    double      sample_rate{stream_history.get_sample_rate()};

    print_ids(fir, differences);

    std::cout << "Number of packets: " << stream_history.get_number_of_packets() << '\n';

    bool integer{(cur->header.tsi != VRT_TSI_NONE && cur->header.tsi == fir->header.tsi)};
    bool fractional{(cur->header.tsf != VRT_TSF_NONE && cur->header.tsf == fir->header.tsf)};
    if (integer && !fractional) {
        print_integer_time(fir, cur);
    } else if (!integer && fractional) {
        print_fractional_time(fir, cur);
    } else {
        print_integer_fractional_time(fir, cur, sample_rate);
    }
}

}  // namespace vrt::length
