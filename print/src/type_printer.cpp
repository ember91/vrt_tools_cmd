#include "type_printer.h"

#include <iostream>
#include <string>

#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_util.h>

#include "stringify.h"

namespace vrt::print {

/**
 * Print formatted GPS/INS geolocation.
 *
 * \param g   Formatted geolocation.
 * \param gps True if GPS geolocation, False otherwise.
 */
static void print_formatted_geolocation(const vrt_formatted_geolocation& g, bool gps) {
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
 * \param e    Ephemeris.
 * \param ecef True if ECEF ephmemeris, false otherwise.
 */
static void print_ephemeris(const vrt_ephemeris& e, bool ecef) {
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
    if (e.has.position_x) {
        WriteCols("position_x [m]", std::to_string(e.position_x), 1);
    }
    if (e.has.position_y) {
        WriteCols("position_y [m]", std::to_string(e.position_y), 1);
    }
    if (e.has.position_z) {
        WriteCols("position_z [m]", std::to_string(e.position_z), 1);
    }
    if (e.has.attitude_alpha) {
        WriteCols("altitude_alpha [degrees]", std::to_string(e.attitude_alpha), 1);
    }
    if (e.has.attitude_beta) {
        WriteCols("altitude_beta [degrees]", std::to_string(e.attitude_beta), 1);
    }
    if (e.has.attitude_phi) {
        WriteCols("altitude_phi [degrees]", std::to_string(e.attitude_phi), 1);
    }
    if (e.has.velocity_dx) {
        WriteCols("velocity_dx [m/s]", std::to_string(e.velocity_dx), 1);
    }
    if (e.has.velocity_dy) {
        WriteCols("velocity_dy [m/s]", std::to_string(e.velocity_dy), 1);
    }
    if (e.has.velocity_dz) {
        WriteCols("velocity_dz [m/s]", std::to_string(e.velocity_dz), 1);
    }
}

/**
 * Print header.
 *
 * \param header Header.
 */
void print_header(const vrt_header& header) {
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
 * \param header Header.
 * \param fields Fields.
 */
void print_fields(const vrt_header& header, const vrt_fields& fields) {
    if (vrt_has_stream_id(&header)) {
        WriteCols("Stream ID", Uint32ToHexStr(fields.stream_id));
    }
    if (header.has.class_id) {
        std::cout << "Class ID\n";
        WriteCols("OUI", Uint32ToHexStr(fields.class_id.oui, 6), 1);
        WriteCols("Information class code", Uint32ToHexStr(fields.class_id.information_class_code, 4), 1);
        WriteCols("Packet class code", Uint32ToHexStr(fields.class_id.packet_class_code, 4), 1);
    }
    if (header.tsi != VRT_TSI_NONE) {
        WriteCols("Integer seconds timestamp", std::to_string(fields.integer_seconds_timestamp));
    }
    if (header.tsf != VRT_TSF_NONE) {
        WriteCols("Fractional seconds timestamp", std::to_string(fields.fractional_seconds_timestamp));
    }
}

/**
 * Print body.
 *
 * \param header     Header.
 * \param words_body Number of words in body.
 */
void print_body(const vrt_header& header, int32_t words_body) {
    switch (header.packet_type) {
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
 * \param if_context IF context.
 */
void print_if_context(const vrt_if_context& if_context) {
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
        print_formatted_geolocation(if_context.formatted_gps_geolocation, true);
    }
    if (if_context.has.formatted_ins_geolocation) {
        print_formatted_geolocation(if_context.formatted_ins_geolocation, false);
    }
    if (if_context.has.ecef_ephemeris) {
        print_ephemeris(if_context.ecef_ephemeris, true);
    }
    if (if_context.has.relative_ephemeris) {
        print_ephemeris(if_context.relative_ephemeris, false);
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
 * \param trailer Trailer.
 */
void print_trailer(const vrt_trailer& trailer) {
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
