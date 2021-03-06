#include "type_printer.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "vrt/vrt_string.h"
#include "vrt/vrt_time.h"
#include "vrt/vrt_types.h"
#include "vrt/vrt_util.h"

#include "stringify.h"

namespace vrt::print {

/**
 * Print calendar time.
 *
 * \param tsf       Timestamp fractional.
 * \param cal_time  Calendar time.
 * \param ind_lvl   Indentation level.
 */
static void print_calendar_time(vrt_tsf tsf, const vrt_calendar_time& cal_time, unsigned int ind_lvl) {
    std::stringstream ss;
    ss << std::setfill('0');
    ss << 1900 + cal_time.year << '-' << std::setw(2) << 1 + cal_time.mon << '-' << std::setw(2) << cal_time.mday << ' '
       << cal_time.hour << ':' << cal_time.min << ':' << cal_time.sec;
    ss << std::setw(0);
    if (tsf != VRT_TSF_NONE) {
        ss << '.' << std::setw(12) << cal_time.ps;
    }
    WriteCols("(Time)", ss.str(), ind_lvl);
}

/**
 * Print formatted GPS/INS geolocation.
 *
 * \param c             IF context.
 * \param sample_rate   Sample rate [Hz].
 * \param gps           True if GPS geolocation, False otherwise.
 */
static void print_formatted_geolocation(const vrt_if_context& c, double sample_rate, bool gps) {
    const vrt_formatted_geolocation& g{gps ? c.formatted_gps_geolocation : c.formatted_ins_geolocation};
    std::cout << "Formatted " << (gps ? "GPS" : "INS") << " geolocation\n";
    WriteCols("TSI", vrt_string_tsi(g.tsi), 1);
    WriteCols("TSF", vrt_string_tsf(g.tsf), 1);
    WriteCols("OUI", Uint32ToHexStr(g.oui, 6), 1);
    if (g.tsi != VRT_TSI_UNDEFINED) {
        WriteCols("Integer second timestamp", std::to_string(g.integer_second_timestamp), 1);
    }
    if (g.tsf != VRT_TSF_UNDEFINED) {
        WriteCols("Fractional second timestamp", std::to_string(g.fractional_second_timestamp), 1);
    }
    vrt_calendar_time cal_time;
    int               rv;
    if (gps) {
        rv = vrt_time_calendar_gps_geolocation(&c, sample_rate, &cal_time);
    } else {
        rv = vrt_time_calendar_ins_geolocation(&c, sample_rate, &cal_time);
    }
    if (rv == 0) {
        print_calendar_time(gps ? c.formatted_gps_geolocation.tsf : c.formatted_ins_geolocation.tsf, cal_time, 1);
    }
    if (g.has.latitude) {
        WriteCols("Latitude [degrees]", std::to_string(g.latitude), 1);
    }
    if (g.has.longitude) {
        WriteCols("Longitude [degrees]", std::to_string(g.longitude), 1);
    }
    if (g.has.altitude) {
        WriteCols("Altitude [m]", std::to_string(g.altitude), 1);
    }
    if (g.has.speed_over_ground) {
        WriteCols("Speed over ground [m/s]", std::to_string(g.speed_over_ground), 1);
    }
    if (g.has.heading_angle) {
        WriteCols("Heading angle [degrees]", std::to_string(g.heading_angle), 1);
    }
    if (g.has.track_angle) {
        WriteCols("Track angle [degrees]", std::to_string(g.track_angle), 1);
    }
    if (g.has.magnetic_variation) {
        WriteCols("Magnetic variation [degrees]", std::to_string(g.magnetic_variation), 1);
    }
}

/**
 * Print ECEF/Relative ephemeris.
 *
 * \param c           IF context.
 * \param sample_rate Sample rate [Hz].
 * \param ecef        True if ECEF ephmemeris, false otherwise.
 */
static void print_ephemeris(const vrt_if_context& c, double sample_rate, bool ecef) {
    const vrt_ephemeris& e{ecef ? c.ecef_ephemeris : c.relative_ephemeris};
    std::cout << (ecef ? "ECEF" : "Relative") << " ephemeris\n";
    WriteCols("TSI", vrt_string_tsi(e.tsi), 1);
    WriteCols("TSF", vrt_string_tsf(e.tsf), 1);
    WriteCols("OUI", Uint32ToHexStr(e.oui, 6), 1);
    if (e.tsi != VRT_TSI_UNDEFINED) {
        WriteCols("Integer second timestamp", std::to_string(e.integer_second_timestamp), 1);
    }
    if (e.tsf != VRT_TSF_UNDEFINED) {
        WriteCols("Fractional second timestamp", std::to_string(e.fractional_second_timestamp), 1);
    }
    vrt_calendar_time cal_time;
    int               rv;
    if (ecef) {
        rv = vrt_time_calendar_ecef_ephemeris(&c, sample_rate, &cal_time);
    } else {
        rv = vrt_time_calendar_relative_ephemeris(&c, sample_rate, &cal_time);
    }
    if (rv == 0) {
        print_calendar_time(ecef ? c.ecef_ephemeris.tsf : c.relative_ephemeris.tsf, cal_time, 1);
    }
    if (e.has.position_x) {
        WriteCols("Position X [m]", std::to_string(e.position_x), 1);
    }
    if (e.has.position_y) {
        WriteCols("Position Y [m]", std::to_string(e.position_y), 1);
    }
    if (e.has.position_z) {
        WriteCols("Position Z [m]", std::to_string(e.position_z), 1);
    }
    if (e.has.attitude_alpha) {
        WriteCols("Altitude alpha [degrees]", std::to_string(e.attitude_alpha), 1);
    }
    if (e.has.attitude_beta) {
        WriteCols("Altitude beta [degrees]", std::to_string(e.attitude_beta), 1);
    }
    if (e.has.attitude_phi) {
        WriteCols("Altitude phi [degrees]", std::to_string(e.attitude_phi), 1);
    }
    if (e.has.velocity_dx) {
        WriteCols("Velocity dX [m/s]", std::to_string(e.velocity_dx), 1);
    }
    if (e.has.velocity_dy) {
        WriteCols("Velocity dY [m/s]", std::to_string(e.velocity_dy), 1);
    }
    if (e.has.velocity_dz) {
        WriteCols("Velocity dZ [m/s]", std::to_string(e.velocity_dz), 1);
    }
}

/**
 * Print header.
 *
 * \param packet Packet.
 */
void print_header(const vrt_packet& packet) {
    const vrt_header& header{packet.header};
    WriteCols("Packet type", vrt_string_packet_type(header.packet_type));
    // No idea to print has.class_id and has.trailer
    WriteCols("tsm", vrt_string_tsm(header.tsm));
    WriteCols("TSI", vrt_string_tsi(header.tsi));
    WriteCols("TSF", vrt_string_tsf(header.tsf));
    WriteCols("Packet count", std::to_string(header.packet_count));
    WriteCols("Packet size [words]", std::to_string(header.packet_size));
}

/**
 * Print fields.
 *
 * \param packet      Packet.
 * \param sample_rate Sample rate [Hz].
 */
void print_fields(const vrt_packet& packet, double sample_rate) {
    if (vrt_has_stream_id(&packet.header)) {
        WriteCols("Stream ID", Uint32ToHexStr(packet.fields.stream_id));
    }
    if (packet.header.has.class_id) {
        std::cout << "Class ID\n";
        WriteCols("OUI", Uint32ToHexStr(packet.fields.class_id.oui, 6), 1);
        WriteCols("Information class code", Uint32ToHexStr(packet.fields.class_id.information_class_code, 4), 1);
        WriteCols("Packet class code", Uint32ToHexStr(packet.fields.class_id.packet_class_code, 4), 1);
    }
    if (packet.header.tsi != VRT_TSI_NONE) {
        WriteCols("Integer seconds timestamp", std::to_string(packet.fields.integer_seconds_timestamp));
    }
    if (packet.header.tsf != VRT_TSF_NONE) {
        WriteCols("Fractional seconds timestamp", std::to_string(packet.fields.fractional_seconds_timestamp));
    }
    vrt_calendar_time cal_time;
    if (vrt_time_calendar_fields(&packet.header, &packet.fields, sample_rate, &cal_time) == 0) {
        print_calendar_time(packet.header.tsf, cal_time, 1);
    }
}

/**
 * Print body.
 *
 * \param packet Packet.
 */
void print_body(const vrt_packet& packet) {
    int32_t words_body{packet.words_body};
    switch (packet.header.packet_type) {
        case VRT_PT_IF_DATA_WITHOUT_STREAM_ID:
        case VRT_PT_IF_DATA_WITH_STREAM_ID:
        case VRT_PT_EXT_DATA_WITHOUT_STREAM_ID:
        case VRT_PT_EXT_DATA_WITH_STREAM_ID: {
            WriteCols("Body size [words]", std::to_string(words_body));
            break;
        }
        case VRT_PT_IF_CONTEXT: {
            // Do nothing
            break;
        }
        case VRT_PT_EXT_CONTEXT: {
            WriteCols("Extended context section size [words]", std::to_string(words_body));
            break;
        }
    }
}

/**
 * Print IF context.
 *
 * \param packet      Packet.
 * \param sample_rate Sample rate [Hz].
 */
void print_if_context(const vrt_packet& packet, double sample_rate) {
    const vrt_if_context& if_context{packet.if_context};
    if (if_context.context_field_change_indicator) {
        WriteCols("Changed", BoolToStr(if_context.context_field_change_indicator));
    }
    if (if_context.has.reference_point_identifier) {
        WriteCols("Reference point identifier", Uint32ToHexStr(if_context.reference_point_identifier));
    }
    if (if_context.has.bandwidth) {
        WriteCols("Bandwidth [Hz]", std::to_string(if_context.bandwidth));
    }
    if (if_context.has.if_reference_frequency) {
        WriteCols("IF reference frequency [Hz]", std::to_string(if_context.if_reference_frequency));
    }
    if (if_context.has.rf_reference_frequency) {
        WriteCols("RF reference frequency [Hz]", std::to_string(if_context.rf_reference_frequency));
    }
    if (if_context.has.rf_reference_frequency_offset) {
        WriteCols("RF reference frequency offset [Hz]", std::to_string(if_context.rf_reference_frequency_offset));
    }
    if (if_context.has.if_band_offset) {
        WriteCols("IF band offset [Hz]", std::to_string(if_context.if_band_offset));
    }
    if (if_context.has.reference_level) {
        WriteCols("Reference level [dBm]", std::to_string(if_context.reference_level));
    }
    if (if_context.has.gain) {
        std::cout << "Gain\n";
        WriteCols("Stage 1 [dB]", std::to_string(if_context.gain.stage1), 1);
        WriteCols("Stage 2 [dB]", std::to_string(if_context.gain.stage2), 1);
    }
    if (if_context.has.over_range_count) {
        WriteCols("Over-range count", std::to_string(if_context.over_range_count));
    }
    if (if_context.has.sample_rate) {
        WriteCols("Sample rate [Hz]", std::to_string(if_context.sample_rate));
    }
    if (if_context.has.timestamp_adjustment) {
        WriteCols("Timestamp adjustment [ps]", std::to_string(if_context.timestamp_adjustment));
    }
    if (if_context.has.timestamp_calibration_time) {
        WriteCols("Timestamp calibration time", std::to_string(if_context.timestamp_calibration_time));
        vrt_calendar_time cal_time;
        if (vrt_time_calendar_calibration(&packet.header, &if_context, &cal_time) == 0) {
            print_calendar_time(VRT_TSF_NONE, cal_time, 1);
        }
    }
    if (if_context.has.temperature) {
        WriteCols("Temperature [degrees C]", std::to_string(if_context.temperature));
    }
    if (if_context.has.device_identifier) {
        std::cout << "Device identifier\n";
        WriteCols("OUI", Uint32ToHexStr(if_context.device_identifier.oui, 6), 1);
        WriteCols("Device code", Uint32ToHexStr(if_context.device_identifier.device_code, 4), 1);
    }
    if (if_context.has.state_and_event_indicators) {
        std::cout << "State and event indicators\n";
        if (if_context.state_and_event_indicators.has.calibrated_time) {
            WriteCols("Calibrated time", BoolToStr(if_context.state_and_event_indicators.calibrated_time), 1);
        }
        if (if_context.state_and_event_indicators.has.valid_data) {
            WriteCols("Valid data", BoolToStr(if_context.state_and_event_indicators.valid_data), 1);
        }
        if (if_context.state_and_event_indicators.has.reference_lock) {
            WriteCols("Reference lock", BoolToStr(if_context.state_and_event_indicators.reference_lock), 1);
        }
        if (if_context.state_and_event_indicators.has.agc_or_mgc) {
            WriteCols("AGC/MGC", vrt_string_agc_or_mgc(if_context.state_and_event_indicators.agc_or_mgc), 1);
        }
        if (if_context.state_and_event_indicators.has.detected_signal) {
            WriteCols("Detected signal", BoolToStr(if_context.state_and_event_indicators.detected_signal), 1);
        }
        if (if_context.state_and_event_indicators.has.spectral_inversion) {
            WriteCols("Spectral inversion", BoolToStr(if_context.state_and_event_indicators.spectral_inversion), 1);
        }
        if (if_context.state_and_event_indicators.has.over_range) {
            WriteCols("Over-range", BoolToStr(if_context.state_and_event_indicators.over_range), 1);
        }
        if (if_context.state_and_event_indicators.has.sample_loss) {
            WriteCols("Sample loss", BoolToStr(if_context.state_and_event_indicators.sample_loss), 1);
        }
        WriteCols("User defined", Uint32ToHexStr(if_context.state_and_event_indicators.user_defined, 2), 1);
    }
    if (if_context.has.data_packet_payload_format) {
        std::cout << "Data packet payload format\n";
        WriteCols("Packing method", vrt_string_packing_method(if_context.data_packet_payload_format.packing_method), 1);
        WriteCols("Real/Complex", vrt_string_real_or_complex(if_context.data_packet_payload_format.real_or_complex), 1);
        WriteCols("Data item format",
                  vrt_string_data_item_format(if_context.data_packet_payload_format.data_item_format), 1);
        WriteCols("Sample component repeat", BoolToStr(if_context.data_packet_payload_format.sample_component_repeat),
                  1);
        WriteCols("Event tag size", std::to_string(if_context.data_packet_payload_format.event_tag_size), 1);
        WriteCols("Channel tag size", std::to_string(if_context.data_packet_payload_format.channel_tag_size), 1);
        WriteCols("Item packing field size",
                  std::to_string(if_context.data_packet_payload_format.item_packing_field_size), 1);
        WriteCols("Data item size", std::to_string(if_context.data_packet_payload_format.data_item_size), 1);
        WriteCols("Repeat count", std::to_string(if_context.data_packet_payload_format.repeat_count), 1);
        WriteCols("Vector size", std::to_string(if_context.data_packet_payload_format.vector_size), 1);
    }
    if (if_context.has.formatted_gps_geolocation) {
        print_formatted_geolocation(if_context, sample_rate, true);
    }
    if (if_context.has.formatted_ins_geolocation) {
        print_formatted_geolocation(if_context, sample_rate, false);
    }
    if (if_context.has.ecef_ephemeris) {
        print_ephemeris(if_context, sample_rate, true);
    }
    if (if_context.has.relative_ephemeris) {
        print_ephemeris(if_context, sample_rate, false);
    }
    if (if_context.has.ephemeris_reference_identifier) {
        WriteCols("Ephemeris reference identifier", Uint32ToHexStr(if_context.ephemeris_reference_identifier));
    }
    if (if_context.has.gps_ascii) {
        std::cout << "GPS ASCII\n";
        WriteCols("OUI", Uint32ToHexStr(if_context.gps_ascii.oui, 6), 1);
        WriteCols("Number of words", std::to_string(if_context.gps_ascii.number_of_words), 1);
        // Get full string and print, even if it may look a bit weird. Useful for debugging.
        std::string ascii_str(if_context.gps_ascii.ascii, if_context.gps_ascii.ascii +
                                                              sizeof(uint32_t) * if_context.gps_ascii.number_of_words -
                                                              if_context.gps_ascii.ascii);
        WriteCols("ASCII", ascii_str, 1);
    }
    if (if_context.has.context_association_lists) {
        std::cout << "Context association lists\n";
        WriteCols("Source list size", std::to_string(if_context.context_association_lists.source_list_size), 1);
        WriteCols("System list size", std::to_string(if_context.context_association_lists.system_list_size), 1);
        WriteCols("Vector component list size",
                  std::to_string(if_context.context_association_lists.vector_component_list_size), 1);
        WriteCols("Asynchronous channel list size",
                  std::to_string(if_context.context_association_lists.asynchronous_channel_list_size), 1);
        if (if_context.context_association_lists.has.asynchronous_channel_tag_list) {
            WriteCols("Asynchronous channel tag list size",
                      std::to_string(if_context.context_association_lists.asynchronous_channel_list_size), 1);
        }

        std::cout << "Source list\n";
        for (uint16_t j{0}; j < if_context.context_association_lists.source_list_size; ++j) {
            WriteCols("", Uint32ToHexStr(if_context.context_association_lists.source_context_association_list[j]), 2);
        }

        std::cout << "System list\n";
        for (uint16_t j{0}; j < if_context.context_association_lists.system_list_size; ++j) {
            WriteCols("", Uint32ToHexStr(if_context.context_association_lists.system_context_association_list[j]), 2);
        }

        std::cout << "Vector component list\n";
        for (uint16_t j{0}; j < if_context.context_association_lists.vector_component_list_size; ++j) {
            WriteCols("",
                      Uint32ToHexStr(if_context.context_association_lists.vector_component_context_association_list[j]),
                      2);
        }

        std::cout << "Asynchronous channel list\n";
        for (uint16_t j{0}; j < if_context.context_association_lists.asynchronous_channel_list_size; ++j) {
            WriteCols(
                "",
                Uint32ToHexStr(if_context.context_association_lists.asynchronous_channel_context_association_list[j]),
                2);
        }

        if (if_context.context_association_lists.has.asynchronous_channel_tag_list) {
            std::cout << "Asynchronous channel tag list\n";
            for (uint16_t j{0}; j < if_context.context_association_lists.asynchronous_channel_list_size; ++j) {
                WriteCols("", Uint32ToHexStr(if_context.context_association_lists.asynchronous_channel_tag_list[j]), 2);
            }
        }
    }
}

/**
 * Print trailer.
 *
 * \param packet Packet.
 */
void print_trailer(const vrt_packet& packet) {
    const vrt_trailer& trailer{packet.trailer};
    if (trailer.has.calibrated_time) {
        WriteCols("Calibrated time", BoolToStr(trailer.calibrated_time));
    }
    if (trailer.has.valid_data) {
        WriteCols("Valid data", BoolToStr(trailer.valid_data));
    }
    if (trailer.has.reference_lock) {
        WriteCols("Reference lock", BoolToStr(trailer.reference_lock));
    }
    if (trailer.has.agc_or_mgc) {
        WriteCols("AGC/MGC", vrt_string_agc_or_mgc(trailer.agc_or_mgc));
    }
    if (trailer.has.detected_signal) {
        WriteCols("Detected signal", BoolToStr(trailer.detected_signal));
    }
    if (trailer.has.spectral_inversion) {
        WriteCols("Spectral inversion", BoolToStr(trailer.spectral_inversion));
    }
    if (trailer.has.over_range) {
        WriteCols("Over range", BoolToStr(trailer.over_range));
    }
    if (trailer.has.sample_loss) {
        WriteCols("Sample loss", BoolToStr(trailer.sample_loss));
    }
    if (trailer.has.user_defined11) {
        WriteCols("User defined 11", BoolToStr(trailer.user_defined11));
    }
    if (trailer.has.user_defined10) {
        WriteCols("User defined 10", BoolToStr(trailer.user_defined10));
    }
    if (trailer.has.user_defined9) {
        WriteCols("User defined 9", BoolToStr(trailer.user_defined9));
    }
    if (trailer.has.user_defined8) {
        WriteCols("User defined 8", BoolToStr(trailer.user_defined8));
    }
    if (trailer.has.associated_context_packet_count) {
        WriteCols("Associated context packet count", std::to_string(trailer.associated_context_packet_count));
    }
}

}  // namespace vrt::print
