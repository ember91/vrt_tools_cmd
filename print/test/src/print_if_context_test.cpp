#include <gtest/gtest.h>

#include <array>
#include <cstdint>

#include "vrt/vrt_init.h"
#include "vrt/vrt_types.h"

#include "../../src/type_printer.h"

static constexpr double SAMPLE_RATE{16e6};

class PrintIfContextTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_packet(&p_); }

    void TearDown() override { vrt::print::print_if_context(p_, SAMPLE_RATE); }

    vrt_packet p_;
};

TEST_F(PrintIfContextTest, None) {}

TEST_F(PrintIfContextTest, ContextFieldChange) {
    p_.if_context.context_field_change_indicator = true;
}

TEST_F(PrintIfContextTest, ReferencePointIdentifier) {
    p_.if_context.has.reference_point_identifier = true;
    p_.if_context.reference_point_identifier     = 0xFEFEFEFE;
}

TEST_F(PrintIfContextTest, Bandwidth) {
    p_.if_context.has.bandwidth = true;
    p_.if_context.bandwidth     = 4097.0;
}

TEST_F(PrintIfContextTest, IfReferenceFrequency) {
    p_.if_context.has.if_reference_frequency = true;
    p_.if_context.if_reference_frequency     = 4097.0;
}

TEST_F(PrintIfContextTest, RfReferenceFrequency) {
    p_.if_context.has.rf_reference_frequency = true;
    p_.if_context.rf_reference_frequency     = 4097.0;
}

TEST_F(PrintIfContextTest, RfReferenceFrequencyOffset) {
    p_.if_context.has.rf_reference_frequency_offset = true;
    p_.if_context.rf_reference_frequency_offset     = 4097.0;
}

TEST_F(PrintIfContextTest, IfBandOffset) {
    p_.if_context.has.if_band_offset = true;
    p_.if_context.if_band_offset     = 4097.0;
}

TEST_F(PrintIfContextTest, ReferenceLevel) {
    p_.if_context.has.reference_level = true;
    p_.if_context.reference_level     = -1.0F;
}

TEST_F(PrintIfContextTest, Gain) {
    p_.if_context.has.gain    = true;
    p_.if_context.gain.stage1 = -1.0F;
    p_.if_context.gain.stage2 = 1.0F;
}

TEST_F(PrintIfContextTest, OverRangeCount) {
    p_.if_context.has.over_range_count = true;
    p_.if_context.over_range_count     = 0xFEFEFEFE;
}

TEST_F(PrintIfContextTest, SampleRate) {
    p_.if_context.has.sample_rate = true;
    p_.if_context.sample_rate     = 4097.0;
}

TEST_F(PrintIfContextTest, TimestampAdjustment) {
    p_.if_context.has.timestamp_adjustment = true;
    p_.if_context.timestamp_adjustment     = 0xABABABABBEBEBEBE;
}

TEST_F(PrintIfContextTest, TimestampCalibrationTime) {
    p_.header.tsi                                = VRT_TSI_UTC;  // Ensure calendar time is printed
    p_.if_context.has.timestamp_calibration_time = true;
    p_.if_context.timestamp_calibration_time     = 1608751092;
}

TEST_F(PrintIfContextTest, Temperature) {
    p_.if_context.has.temperature = true;
    p_.if_context.temperature     = -1.0F;
}

TEST_F(PrintIfContextTest, DeviceIdentifier) {
    p_.if_context.has.device_identifier         = true;
    p_.if_context.device_identifier.oui         = 0xFFFFFFFF;
    p_.if_context.device_identifier.device_code = 0xABAF;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasCalibratedTime) {
    p_.if_context.has.state_and_event_indicators                 = true;
    p_.if_context.state_and_event_indicators.has.calibrated_time = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasValidData) {
    p_.if_context.has.state_and_event_indicators            = true;
    p_.if_context.state_and_event_indicators.has.valid_data = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasReferenceLock) {
    p_.if_context.has.state_and_event_indicators                = true;
    p_.if_context.state_and_event_indicators.has.reference_lock = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasAgcOrMgc) {
    p_.if_context.has.state_and_event_indicators            = true;
    p_.if_context.state_and_event_indicators.has.agc_or_mgc = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasDetectedSignal) {
    p_.if_context.has.state_and_event_indicators                 = true;
    p_.if_context.state_and_event_indicators.has.detected_signal = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasSpectralInversion) {
    p_.if_context.has.state_and_event_indicators                    = true;
    p_.if_context.state_and_event_indicators.has.spectral_inversion = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasOverRange) {
    p_.if_context.has.state_and_event_indicators            = true;
    p_.if_context.state_and_event_indicators.has.over_range = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasSampleLoss) {
    p_.if_context.has.state_and_event_indicators             = true;
    p_.if_context.state_and_event_indicators.has.sample_loss = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsCalibratedTime) {
    p_.if_context.has.state_and_event_indicators             = true;
    p_.if_context.state_and_event_indicators.calibrated_time = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsValidData) {
    p_.if_context.has.state_and_event_indicators        = true;
    p_.if_context.state_and_event_indicators.valid_data = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsReferenceLock) {
    p_.if_context.has.state_and_event_indicators            = true;
    p_.if_context.state_and_event_indicators.reference_lock = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsAgcOrMgc) {
    p_.if_context.has.state_and_event_indicators        = true;
    p_.if_context.state_and_event_indicators.agc_or_mgc = VRT_AOM_AGC;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsDetectedSignal) {
    p_.if_context.has.state_and_event_indicators             = true;
    p_.if_context.state_and_event_indicators.detected_signal = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsSpectralInversion) {
    p_.if_context.has.state_and_event_indicators                = true;
    p_.if_context.state_and_event_indicators.spectral_inversion = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsOverRange) {
    p_.if_context.has.state_and_event_indicators        = true;
    p_.if_context.state_and_event_indicators.over_range = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsSampleLoss) {
    p_.if_context.has.state_and_event_indicators         = true;
    p_.if_context.state_and_event_indicators.sample_loss = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothCalibratedTime) {
    p_.if_context.has.state_and_event_indicators                 = true;
    p_.if_context.state_and_event_indicators.has.calibrated_time = true;
    p_.if_context.state_and_event_indicators.calibrated_time     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothValidData) {
    p_.if_context.has.state_and_event_indicators            = true;
    p_.if_context.state_and_event_indicators.has.valid_data = true;
    p_.if_context.state_and_event_indicators.valid_data     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothReferenceLock) {
    p_.if_context.has.state_and_event_indicators                = true;
    p_.if_context.state_and_event_indicators.has.reference_lock = true;
    p_.if_context.state_and_event_indicators.reference_lock     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothAgcOrMgc) {
    p_.if_context.has.state_and_event_indicators            = true;
    p_.if_context.state_and_event_indicators.has.agc_or_mgc = true;
    p_.if_context.state_and_event_indicators.agc_or_mgc     = VRT_AOM_AGC;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothDetectedSignal) {
    p_.if_context.has.state_and_event_indicators                 = true;
    p_.if_context.state_and_event_indicators.has.detected_signal = true;
    p_.if_context.state_and_event_indicators.detected_signal     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothSpectralInversion) {
    p_.if_context.has.state_and_event_indicators                    = true;
    p_.if_context.state_and_event_indicators.has.spectral_inversion = true;
    p_.if_context.state_and_event_indicators.spectral_inversion     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothOverRange) {
    p_.if_context.has.state_and_event_indicators            = true;
    p_.if_context.state_and_event_indicators.has.over_range = true;
    p_.if_context.state_and_event_indicators.over_range     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothSampleLoss) {
    p_.if_context.has.state_and_event_indicators             = true;
    p_.if_context.state_and_event_indicators.has.sample_loss = true;
    p_.if_context.state_and_event_indicators.sample_loss     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothUserDefined) {
    p_.if_context.has.state_and_event_indicators          = true;
    p_.if_context.state_and_event_indicators.user_defined = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatPackingMethod) {
    p_.if_context.has.data_packet_payload_format            = true;
    p_.if_context.data_packet_payload_format.packing_method = VRT_PM_LINK_EFFICIENT;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatRealOrComplex1) {
    p_.if_context.has.data_packet_payload_format             = true;
    p_.if_context.data_packet_payload_format.real_or_complex = VRT_ROC_COMPLEX_POLAR;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatRealOrComplex2) {
    p_.if_context.has.data_packet_payload_format             = true;
    p_.if_context.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(3);
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatDataItemFormat1) {
    p_.if_context.has.data_packet_payload_format              = true;
    p_.if_context.data_packet_payload_format.data_item_format = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatDataItemFormat2) {
    p_.if_context.has.data_packet_payload_format              = true;
    p_.if_context.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(31);
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatSampleComponentRepeat) {
    p_.if_context.has.data_packet_payload_format                     = true;
    p_.if_context.data_packet_payload_format.sample_component_repeat = true;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatEventTagSize) {
    p_.if_context.has.data_packet_payload_format            = true;
    p_.if_context.data_packet_payload_format.event_tag_size = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatChannelTagSize) {
    p_.if_context.has.data_packet_payload_format              = true;
    p_.if_context.data_packet_payload_format.channel_tag_size = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatItemPackingFieldSize) {
    p_.if_context.has.data_packet_payload_format                     = true;
    p_.if_context.data_packet_payload_format.item_packing_field_size = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatDataItemSize) {
    p_.if_context.has.data_packet_payload_format            = true;
    p_.if_context.data_packet_payload_format.data_item_size = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatRepeatCount) {
    p_.if_context.has.data_packet_payload_format          = true;
    p_.if_context.data_packet_payload_format.repeat_count = 0xFFFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatVectorSize) {
    p_.if_context.has.data_packet_payload_format         = true;
    p_.if_context.data_packet_payload_format.vector_size = 0xFFFF;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationTsi) {
    p_.if_context.has.formatted_gps_geolocation = true;
    p_.if_context.formatted_gps_geolocation.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationTsf) {
    p_.if_context.has.formatted_gps_geolocation = true;
    p_.if_context.formatted_gps_geolocation.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationOui) {
    p_.if_context.has.formatted_gps_geolocation = true;
    p_.if_context.formatted_gps_geolocation.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationIntegerSecondTimestamp) {
    p_.if_context.has.formatted_gps_geolocation                      = true;
    p_.if_context.formatted_gps_geolocation.tsi                      = VRT_TSI_UTC;
    p_.if_context.formatted_gps_geolocation.integer_second_timestamp = 1608751092;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationFractionalSecondTimestamp) {
    p_.if_context.has.formatted_gps_geolocation                         = true;
    p_.if_context.formatted_gps_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    p_.if_context.formatted_gps_geolocation.fractional_second_timestamp = 0xABABABABBABABABA;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationIntegerFractionalSecondTimestamp) {
    p_.if_context.has.formatted_gps_geolocation                         = true;
    p_.if_context.formatted_gps_geolocation.tsi                         = VRT_TSI_UTC;
    p_.if_context.formatted_gps_geolocation.tsf                         = VRT_TSF_SAMPLE_COUNT;
    p_.if_context.formatted_gps_geolocation.integer_second_timestamp    = 1608751092;
    p_.if_context.formatted_gps_geolocation.fractional_second_timestamp = 8e6;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationLatitude) {
    p_.if_context.has.formatted_gps_geolocation      = true;
    p_.if_context.formatted_gps_geolocation.latitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationLongitude) {
    p_.if_context.has.formatted_gps_geolocation       = true;
    p_.if_context.formatted_gps_geolocation.longitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationAltitude) {
    p_.if_context.has.formatted_gps_geolocation      = true;
    p_.if_context.formatted_gps_geolocation.altitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationSpeedOverGround) {
    p_.if_context.has.formatted_gps_geolocation               = true;
    p_.if_context.formatted_gps_geolocation.speed_over_ground = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationHeadingAngle) {
    p_.if_context.has.formatted_gps_geolocation           = true;
    p_.if_context.formatted_gps_geolocation.heading_angle = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationTrackAngle) {
    p_.if_context.has.formatted_gps_geolocation         = true;
    p_.if_context.formatted_gps_geolocation.track_angle = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationMagneticVariation) {
    p_.if_context.has.formatted_gps_geolocation                = true;
    p_.if_context.formatted_gps_geolocation.magnetic_variation = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationTsi) {
    p_.if_context.has.formatted_ins_geolocation = true;
    p_.if_context.formatted_ins_geolocation.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationTsf) {
    p_.if_context.has.formatted_ins_geolocation = true;
    p_.if_context.formatted_ins_geolocation.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationOui) {
    p_.if_context.has.formatted_ins_geolocation = true;
    p_.if_context.formatted_ins_geolocation.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationIntegerSecondTimestamp) {
    p_.if_context.has.formatted_ins_geolocation                      = true;
    p_.if_context.formatted_ins_geolocation.tsi                      = VRT_TSI_UTC;
    p_.if_context.formatted_ins_geolocation.integer_second_timestamp = 1608751092;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationFractionalSecondTimestamp) {
    p_.if_context.has.formatted_ins_geolocation                         = true;
    p_.if_context.formatted_ins_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    p_.if_context.formatted_ins_geolocation.fractional_second_timestamp = 0xABABABABBABABABA;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationIntegerFractionalSecondTimestamp) {
    p_.if_context.has.formatted_ins_geolocation                         = true;
    p_.if_context.formatted_ins_geolocation.tsi                         = VRT_TSI_UTC;
    p_.if_context.formatted_ins_geolocation.tsf                         = VRT_TSF_SAMPLE_COUNT;
    p_.if_context.formatted_ins_geolocation.integer_second_timestamp    = 1608751092;
    p_.if_context.formatted_ins_geolocation.fractional_second_timestamp = 8e6;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationLatitude) {
    p_.if_context.has.formatted_ins_geolocation      = true;
    p_.if_context.formatted_ins_geolocation.latitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationLongitude) {
    p_.if_context.has.formatted_ins_geolocation       = true;
    p_.if_context.formatted_ins_geolocation.longitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationAltitude) {
    p_.if_context.has.formatted_ins_geolocation      = true;
    p_.if_context.formatted_ins_geolocation.altitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationSpeedOverGround) {
    p_.if_context.has.formatted_ins_geolocation               = true;
    p_.if_context.formatted_ins_geolocation.speed_over_ground = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationHeadingAngle) {
    p_.if_context.has.formatted_ins_geolocation           = true;
    p_.if_context.formatted_ins_geolocation.heading_angle = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationTrackAngle) {
    p_.if_context.has.formatted_ins_geolocation         = true;
    p_.if_context.formatted_ins_geolocation.track_angle = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationMagneticVariation) {
    p_.if_context.has.formatted_ins_geolocation                = true;
    p_.if_context.formatted_ins_geolocation.magnetic_variation = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisTsi) {
    p_.if_context.has.ecef_ephemeris = true;
    p_.if_context.ecef_ephemeris.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintIfContextTest, EcefEphemerisTsf) {
    p_.if_context.has.ecef_ephemeris = true;
    p_.if_context.ecef_ephemeris.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintIfContextTest, EcefEphemerisOui) {
    p_.if_context.has.ecef_ephemeris = true;
    p_.if_context.ecef_ephemeris.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, EcefEphemerisIntegerSecondTimestamp) {
    p_.if_context.has.ecef_ephemeris                      = true;
    p_.if_context.ecef_ephemeris.tsi                      = VRT_TSI_UTC;
    p_.if_context.ecef_ephemeris.integer_second_timestamp = 1608751092;
}

TEST_F(PrintIfContextTest, EcefEphemerisFractionalSecondTimestamp) {
    p_.if_context.has.ecef_ephemeris                         = true;
    p_.if_context.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.ecef_ephemeris.fractional_second_timestamp = 0xABABABABBABABABA;
}

TEST_F(PrintIfContextTest, EcefEphemerisIntegerFractionalSecondTimestamp) {
    p_.if_context.has.ecef_ephemeris                         = true;
    p_.if_context.ecef_ephemeris.tsi                         = VRT_TSI_UTC;
    p_.if_context.ecef_ephemeris.tsf                         = VRT_TSF_SAMPLE_COUNT;
    p_.if_context.ecef_ephemeris.integer_second_timestamp    = 1608751092;
    p_.if_context.ecef_ephemeris.fractional_second_timestamp = 8e6;
}

TEST_F(PrintIfContextTest, EcefEphemerisPositionX) {
    p_.if_context.has.ecef_ephemeris        = true;
    p_.if_context.ecef_ephemeris.position_x = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisPositionY) {
    p_.if_context.has.ecef_ephemeris        = true;
    p_.if_context.ecef_ephemeris.position_y = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisPositionZ) {
    p_.if_context.has.ecef_ephemeris        = true;
    p_.if_context.ecef_ephemeris.position_z = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisAltitudeAlpha) {
    p_.if_context.has.ecef_ephemeris            = true;
    p_.if_context.ecef_ephemeris.attitude_alpha = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisAltitudeBeta) {
    p_.if_context.has.ecef_ephemeris           = true;
    p_.if_context.ecef_ephemeris.attitude_beta = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisAltitudePhi) {
    p_.if_context.has.ecef_ephemeris          = true;
    p_.if_context.ecef_ephemeris.attitude_phi = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisVelocityDx) {
    p_.if_context.has.ecef_ephemeris         = true;
    p_.if_context.ecef_ephemeris.velocity_dx = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisVelocityDy) {
    p_.if_context.has.ecef_ephemeris         = true;
    p_.if_context.ecef_ephemeris.velocity_dy = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisVelocityDz) {
    p_.if_context.has.ecef_ephemeris         = true;
    p_.if_context.ecef_ephemeris.velocity_dz = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisTsi) {
    p_.if_context.has.relative_ephemeris = true;
    p_.if_context.relative_ephemeris.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintIfContextTest, RelativeEphemerisTsf) {
    p_.if_context.has.relative_ephemeris = true;
    p_.if_context.relative_ephemeris.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintIfContextTest, RelativeEphemerisOui) {
    p_.if_context.has.relative_ephemeris = true;
    p_.if_context.relative_ephemeris.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, RelativeEphemerisIntegerSecondTimestamp) {
    p_.if_context.has.relative_ephemeris                      = true;
    p_.if_context.relative_ephemeris.tsi                      = VRT_TSI_UTC;
    p_.if_context.relative_ephemeris.integer_second_timestamp = 1608751092;
}

TEST_F(PrintIfContextTest, RelativeEphemerisFractionalSecondTimestamp) {
    p_.if_context.has.relative_ephemeris                         = true;
    p_.if_context.relative_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    p_.if_context.relative_ephemeris.fractional_second_timestamp = 0xABABABABBABABABA;
}

TEST_F(PrintIfContextTest, RelativeEphemerisIntegerFractionalSecondTimestamp) {
    p_.if_context.has.relative_ephemeris                         = true;
    p_.if_context.relative_ephemeris.tsi                         = VRT_TSI_UTC;
    p_.if_context.relative_ephemeris.tsf                         = VRT_TSF_SAMPLE_COUNT;
    p_.if_context.relative_ephemeris.integer_second_timestamp    = 1608751092;
    p_.if_context.relative_ephemeris.fractional_second_timestamp = 8e6;
}

TEST_F(PrintIfContextTest, RelativeEphemerisPositionX) {
    p_.if_context.has.relative_ephemeris        = true;
    p_.if_context.relative_ephemeris.position_x = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisPositionY) {
    p_.if_context.has.relative_ephemeris        = true;
    p_.if_context.relative_ephemeris.position_y = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisPositionZ) {
    p_.if_context.has.relative_ephemeris        = true;
    p_.if_context.relative_ephemeris.position_z = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisAltitudeAlpha) {
    p_.if_context.has.relative_ephemeris            = true;
    p_.if_context.relative_ephemeris.attitude_alpha = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisAltitudeBeta) {
    p_.if_context.has.relative_ephemeris           = true;
    p_.if_context.relative_ephemeris.attitude_beta = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisAltitudePhi) {
    p_.if_context.has.relative_ephemeris          = true;
    p_.if_context.relative_ephemeris.attitude_phi = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisVelocityDx) {
    p_.if_context.has.relative_ephemeris         = true;
    p_.if_context.relative_ephemeris.velocity_dx = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisVelocityDy) {
    p_.if_context.has.relative_ephemeris         = true;
    p_.if_context.relative_ephemeris.velocity_dy = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisVelocityDz) {
    p_.if_context.has.relative_ephemeris         = true;
    p_.if_context.relative_ephemeris.velocity_dz = 1.0;
}

TEST_F(PrintIfContextTest, EphemerisReferenceIdentifier) {
    p_.if_context.has.ephemeris_reference_identifier = true;
    p_.if_context.ephemeris_reference_identifier     = 0xFEFEFEFE;
}

TEST_F(PrintIfContextTest, GpsAsciiOui) {
    p_.if_context.has.gps_ascii = true;
    p_.if_context.gps_ascii.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, GpsAsciiAscii) {
    p_.if_context.has.gps_ascii             = true;
    p_.if_context.gps_ascii.number_of_words = 5;
    p_.if_context.gps_ascii.ascii           = "What's your name?";
}

TEST_F(PrintIfContextTest, ContextAssociationListsSource) {
    p_.if_context.has.context_association_lists                             = true;
    p_.if_context.context_association_lists.source_list_size                = 6;
    std::array<uint32_t, 6> l                                               = {};
    l[0]                                                                    = 0xABABABAB;
    l[1]                                                                    = 0xBCBCBCBC;
    l[2]                                                                    = 0xCDCDCDCD;
    l[3]                                                                    = 0xDEDEDEDE;
    l[4]                                                                    = 0xEFEFEFEF;
    l[5]                                                                    = 0x0F0F0F0F;
    p_.if_context.context_association_lists.source_context_association_list = l.data();
}

TEST_F(PrintIfContextTest, ContextAssociationListsSystem) {
    p_.if_context.has.context_association_lists                             = true;
    p_.if_context.context_association_lists.system_list_size                = 5;
    std::array<uint32_t, 5> l                                               = {};
    l[0]                                                                    = 0xABABABAB;
    l[1]                                                                    = 0xBCBCBCBC;
    l[2]                                                                    = 0xCDCDCDCD;
    l[3]                                                                    = 0xDEDEDEDE;
    l[4]                                                                    = 0xEFEFEFEF;
    p_.if_context.context_association_lists.system_context_association_list = l.data();
}

TEST_F(PrintIfContextTest, ContextAssociationListsVectorComponent) {
    p_.if_context.has.context_association_lists                                       = true;
    p_.if_context.context_association_lists.vector_component_list_size                = 4;
    std::array<uint32_t, 4> l                                                         = {};
    l[0]                                                                              = 0xABABABAB;
    l[1]                                                                              = 0xBCBCBCBC;
    l[2]                                                                              = 0xCDCDCDCD;
    l[3]                                                                              = 0xDEDEDEDE;
    p_.if_context.context_association_lists.vector_component_context_association_list = l.data();
}

TEST_F(PrintIfContextTest, ContextAssociationListsAsynchronous) {
    p_.if_context.has.context_association_lists                                           = true;
    p_.if_context.context_association_lists.asynchronous_channel_list_size                = 3;
    std::array<uint32_t, 3> l                                                             = {};
    l[0]                                                                                  = 0xABABABAB;
    l[1]                                                                                  = 0xBCBCBCBC;
    l[2]                                                                                  = 0xCDCDCDCD;
    p_.if_context.context_association_lists.asynchronous_channel_context_association_list = l.data();
}

TEST_F(PrintIfContextTest, ContextAssociationListsAsynchronousTag) {
    p_.if_context.has.context_association_lists                                           = true;
    p_.if_context.context_association_lists.asynchronous_channel_list_size                = 2;
    p_.if_context.context_association_lists.has.asynchronous_channel_tag_list             = true;
    std::array<uint32_t, 2> l1                                                            = {};
    l1[0]                                                                                 = 0xABABABAB;
    l1[1]                                                                                 = 0xBCBCBCBC;
    std::array<uint32_t, 2> l2                                                            = {};
    l2[0]                                                                                 = 0xABABABAB;
    l2[1]                                                                                 = 0xBCBCBCBC;
    p_.if_context.context_association_lists.asynchronous_channel_context_association_list = l1.data();
    p_.if_context.context_association_lists.asynchronous_channel_tag_list                 = l2.data();
}

TEST_F(PrintIfContextTest, EveryOther1) {
    p_.if_context.context_field_change_indicator = true;
    p_.if_context.has.bandwidth                  = true;
    p_.if_context.has.rf_reference_frequency     = true;
    p_.if_context.has.if_band_offset             = true;
    p_.if_context.has.gain                       = true;
    p_.if_context.has.sample_rate                = true;
    p_.if_context.has.timestamp_calibration_time = true;
    p_.if_context.has.device_identifier          = true;
    p_.if_context.has.data_packet_payload_format = true;
    p_.if_context.has.formatted_ins_geolocation  = true;
    p_.if_context.has.relative_ephemeris         = true;
    p_.if_context.has.gps_ascii                  = true;

    p_.if_context.bandwidth                     = -1.0;
    p_.if_context.rf_reference_frequency        = -1.0;
    p_.if_context.if_band_offset                = -1.0;
    p_.if_context.gain.stage1                   = -1.0F;
    p_.if_context.gain.stage2                   = -1.0F;
    p_.if_context.sample_rate                   = -1.0;
    p_.if_context.timestamp_calibration_time    = 0xBABABABA;
    p_.if_context.device_identifier.oui         = 0xFEBEFEBE;
    p_.if_context.device_identifier.device_code = 0xBEBA;

    p_.if_context.data_packet_payload_format.packing_method          = VRT_PM_LINK_EFFICIENT;
    p_.if_context.data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    p_.if_context.data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    p_.if_context.data_packet_payload_format.sample_component_repeat = true;
    p_.if_context.data_packet_payload_format.event_tag_size          = 0xFF;
    p_.if_context.data_packet_payload_format.channel_tag_size        = 0xFF;
    p_.if_context.data_packet_payload_format.item_packing_field_size = 0xFF;
    p_.if_context.data_packet_payload_format.data_item_size          = 0xFF;
    p_.if_context.data_packet_payload_format.repeat_count            = 0xFFFF;
    p_.if_context.data_packet_payload_format.vector_size             = 0xFFFF;

    p_.if_context.formatted_ins_geolocation.tsi                         = VRT_TSI_OTHER;
    p_.if_context.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.formatted_ins_geolocation.oui                         = 0xFFFFFFFF;
    p_.if_context.formatted_ins_geolocation.integer_second_timestamp    = 0xABABBABA;
    p_.if_context.formatted_ins_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    p_.if_context.formatted_ins_geolocation.latitude                    = -1.0;
    p_.if_context.formatted_ins_geolocation.longitude                   = -1.0;
    p_.if_context.formatted_ins_geolocation.altitude                    = -1.0;
    p_.if_context.formatted_ins_geolocation.speed_over_ground           = -1.0;
    p_.if_context.formatted_ins_geolocation.heading_angle               = -1.0;
    p_.if_context.formatted_ins_geolocation.track_angle                 = -1.0;
    p_.if_context.formatted_ins_geolocation.magnetic_variation          = -1.0;

    p_.if_context.relative_ephemeris.tsi                         = VRT_TSI_OTHER;
    p_.if_context.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.relative_ephemeris.oui                         = 0xFFFFFFFF;
    p_.if_context.relative_ephemeris.integer_second_timestamp    = 0xABABBABA;
    p_.if_context.relative_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    p_.if_context.relative_ephemeris.position_x                  = -1.0;
    p_.if_context.relative_ephemeris.position_y                  = -1.0;
    p_.if_context.relative_ephemeris.position_z                  = -1.0;
    p_.if_context.relative_ephemeris.attitude_alpha              = -1.0;
    p_.if_context.relative_ephemeris.attitude_beta               = -1.0;
    p_.if_context.relative_ephemeris.attitude_phi                = -1.0;
    p_.if_context.relative_ephemeris.velocity_dx                 = -1.0;
    p_.if_context.relative_ephemeris.velocity_dy                 = -1.0;
    p_.if_context.relative_ephemeris.velocity_dz                 = -1.0;

    p_.if_context.gps_ascii.oui             = 0xFFFFFFFF;
    p_.if_context.gps_ascii.number_of_words = 3;
    p_.if_context.gps_ascii.ascii           = "Lorem ipsu";
}

TEST_F(PrintIfContextTest, EveryOther2) {
    p_.if_context.has.reference_point_identifier     = true;
    p_.if_context.has.if_reference_frequency         = true;
    p_.if_context.has.rf_reference_frequency_offset  = true;
    p_.if_context.has.reference_level                = true;
    p_.if_context.has.over_range_count               = true;
    p_.if_context.has.timestamp_adjustment           = true;
    p_.if_context.has.temperature                    = true;
    p_.if_context.has.state_and_event_indicators     = true;
    p_.if_context.has.formatted_gps_geolocation      = true;
    p_.if_context.has.ecef_ephemeris                 = true;
    p_.if_context.has.ephemeris_reference_identifier = true;
    p_.if_context.has.context_association_lists      = true;

    p_.if_context.reference_point_identifier    = 0xACACACAC;
    p_.if_context.if_reference_frequency        = -1.0;
    p_.if_context.rf_reference_frequency_offset = -1.0;
    p_.if_context.reference_level               = -1.0F;
    p_.if_context.over_range_count              = 0xBDBDBDBD;
    p_.if_context.timestamp_adjustment          = 0xABCABCABCABCABCA;
    p_.if_context.temperature                   = -1.0F;

    p_.if_context.state_and_event_indicators.has.calibrated_time    = true;
    p_.if_context.state_and_event_indicators.has.valid_data         = true;
    p_.if_context.state_and_event_indicators.has.reference_lock     = true;
    p_.if_context.state_and_event_indicators.has.agc_or_mgc         = true;
    p_.if_context.state_and_event_indicators.has.detected_signal    = true;
    p_.if_context.state_and_event_indicators.has.spectral_inversion = true;
    p_.if_context.state_and_event_indicators.has.over_range         = true;
    p_.if_context.state_and_event_indicators.has.sample_loss        = true;
    p_.if_context.state_and_event_indicators.calibrated_time        = true;
    p_.if_context.state_and_event_indicators.valid_data             = true;
    p_.if_context.state_and_event_indicators.reference_lock         = true;
    p_.if_context.state_and_event_indicators.agc_or_mgc             = VRT_AOM_AGC;
    p_.if_context.state_and_event_indicators.detected_signal        = true;
    p_.if_context.state_and_event_indicators.spectral_inversion     = true;
    p_.if_context.state_and_event_indicators.over_range             = true;
    p_.if_context.state_and_event_indicators.sample_loss            = true;
    p_.if_context.state_and_event_indicators.user_defined           = 0xFF;

    p_.if_context.formatted_gps_geolocation.tsi                         = VRT_TSI_OTHER;
    p_.if_context.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.formatted_gps_geolocation.oui                         = 0xFFFFFFFF;
    p_.if_context.formatted_gps_geolocation.integer_second_timestamp    = 0xABABBABA;
    p_.if_context.formatted_gps_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    p_.if_context.formatted_gps_geolocation.latitude                    = -1.0;
    p_.if_context.formatted_gps_geolocation.longitude                   = -1.0;
    p_.if_context.formatted_gps_geolocation.altitude                    = -1.0;
    p_.if_context.formatted_gps_geolocation.speed_over_ground           = -1.0;
    p_.if_context.formatted_gps_geolocation.heading_angle               = -1.0;
    p_.if_context.formatted_gps_geolocation.track_angle                 = -1.0;
    p_.if_context.formatted_gps_geolocation.magnetic_variation          = -1.0;

    p_.if_context.ecef_ephemeris.tsi                         = VRT_TSI_OTHER;
    p_.if_context.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.ecef_ephemeris.oui                         = 0xFFFFFFFF;
    p_.if_context.ecef_ephemeris.integer_second_timestamp    = 0xABABBABA;
    p_.if_context.ecef_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    p_.if_context.ecef_ephemeris.position_x                  = -1.0;
    p_.if_context.ecef_ephemeris.position_y                  = -1.0;
    p_.if_context.ecef_ephemeris.position_z                  = -1.0;
    p_.if_context.ecef_ephemeris.attitude_alpha              = -1.0;
    p_.if_context.ecef_ephemeris.attitude_beta               = -1.0;
    p_.if_context.ecef_ephemeris.attitude_phi                = -1.0;
    p_.if_context.ecef_ephemeris.velocity_dx                 = -1.0;
    p_.if_context.ecef_ephemeris.velocity_dy                 = -1.0;
    p_.if_context.ecef_ephemeris.velocity_dz                 = -1.0;

    p_.if_context.ephemeris_reference_identifier = 0xDEDEDEDE;

    std::array<uint32_t, 2> l1                                                            = {0x12345678, 0x23456789};
    std::array<uint32_t, 2> l2                                                            = {0x3456789A, 0x456789AB};
    std::array<uint32_t, 2> l3                                                            = {0x56789ABC, 0x6789ABCD};
    std::array<uint32_t, 2> l4                                                            = {0x789ABCDE, 0x89ABCDEF};
    std::array<uint32_t, 2> l5                                                            = {0x9ABCDEF0, 0xABCDEF01};
    p_.if_context.context_association_lists.has.asynchronous_channel_tag_list             = true;
    p_.if_context.context_association_lists.source_list_size                              = l1.size();
    p_.if_context.context_association_lists.system_list_size                              = l2.size();
    p_.if_context.context_association_lists.vector_component_list_size                    = l3.size();
    p_.if_context.context_association_lists.asynchronous_channel_list_size                = l4.size();
    p_.if_context.context_association_lists.source_context_association_list               = l1.data();
    p_.if_context.context_association_lists.system_context_association_list               = l2.data();
    p_.if_context.context_association_lists.vector_component_context_association_list     = l3.data();
    p_.if_context.context_association_lists.asynchronous_channel_context_association_list = l4.data();
    p_.if_context.context_association_lists.asynchronous_channel_tag_list                 = l5.data();
}

TEST_F(PrintIfContextTest, All) {
    p_.if_context.context_field_change_indicator     = true;
    p_.if_context.has.reference_point_identifier     = true;
    p_.if_context.has.bandwidth                      = true;
    p_.if_context.has.if_reference_frequency         = true;
    p_.if_context.has.rf_reference_frequency         = true;
    p_.if_context.has.rf_reference_frequency_offset  = true;
    p_.if_context.has.if_band_offset                 = true;
    p_.if_context.has.reference_level                = true;
    p_.if_context.has.gain                           = true;
    p_.if_context.has.over_range_count               = true;
    p_.if_context.has.sample_rate                    = true;
    p_.if_context.has.timestamp_adjustment           = true;
    p_.if_context.has.timestamp_calibration_time     = true;
    p_.if_context.has.temperature                    = true;
    p_.if_context.has.device_identifier              = true;
    p_.if_context.has.state_and_event_indicators     = true;
    p_.if_context.has.data_packet_payload_format     = true;
    p_.if_context.has.formatted_gps_geolocation      = true;
    p_.if_context.has.formatted_ins_geolocation      = true;
    p_.if_context.has.ecef_ephemeris                 = true;
    p_.if_context.has.relative_ephemeris             = true;
    p_.if_context.has.ephemeris_reference_identifier = true;
    p_.if_context.has.gps_ascii                      = true;
    p_.if_context.has.context_association_lists      = true;

    p_.if_context.reference_point_identifier    = 0xACACACAC;
    p_.if_context.bandwidth                     = -1.0;
    p_.if_context.if_reference_frequency        = -1.0;
    p_.if_context.rf_reference_frequency        = -1.0;
    p_.if_context.rf_reference_frequency_offset = -1.0;
    p_.if_context.if_band_offset                = -1.0;
    p_.if_context.reference_level               = -1.0F;
    p_.if_context.gain.stage1                   = -1.0F;
    p_.if_context.gain.stage2                   = -1.0F;
    p_.if_context.over_range_count              = 0xBDBDBDBD;
    p_.if_context.sample_rate                   = -1.0;
    p_.if_context.timestamp_adjustment          = 0xABCABCABCABCABCA;
    p_.if_context.timestamp_calibration_time    = 0xBABABABA;
    p_.if_context.temperature                   = -1.0F;
    p_.if_context.device_identifier.oui         = 0xFEBEFEBE;
    p_.if_context.device_identifier.device_code = 0xBEBA;

    p_.if_context.state_and_event_indicators.has.calibrated_time    = true;
    p_.if_context.state_and_event_indicators.has.valid_data         = true;
    p_.if_context.state_and_event_indicators.has.reference_lock     = true;
    p_.if_context.state_and_event_indicators.has.agc_or_mgc         = true;
    p_.if_context.state_and_event_indicators.has.detected_signal    = true;
    p_.if_context.state_and_event_indicators.has.spectral_inversion = true;
    p_.if_context.state_and_event_indicators.has.over_range         = true;
    p_.if_context.state_and_event_indicators.has.sample_loss        = true;
    p_.if_context.state_and_event_indicators.calibrated_time        = true;
    p_.if_context.state_and_event_indicators.valid_data             = true;
    p_.if_context.state_and_event_indicators.reference_lock         = true;
    p_.if_context.state_and_event_indicators.agc_or_mgc             = VRT_AOM_AGC;
    p_.if_context.state_and_event_indicators.detected_signal        = true;
    p_.if_context.state_and_event_indicators.spectral_inversion     = true;
    p_.if_context.state_and_event_indicators.over_range             = true;
    p_.if_context.state_and_event_indicators.sample_loss            = true;
    p_.if_context.state_and_event_indicators.user_defined           = 0xFF;

    p_.if_context.data_packet_payload_format.packing_method          = VRT_PM_LINK_EFFICIENT;
    p_.if_context.data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    p_.if_context.data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    p_.if_context.data_packet_payload_format.sample_component_repeat = true;
    p_.if_context.data_packet_payload_format.event_tag_size          = 0xFF;
    p_.if_context.data_packet_payload_format.channel_tag_size        = 0xFF;
    p_.if_context.data_packet_payload_format.item_packing_field_size = 0xFF;
    p_.if_context.data_packet_payload_format.data_item_size          = 0xFF;
    p_.if_context.data_packet_payload_format.repeat_count            = 0xFFFF;
    p_.if_context.data_packet_payload_format.vector_size             = 0xFFFF;

    p_.if_context.formatted_gps_geolocation.tsi                         = VRT_TSI_OTHER;
    p_.if_context.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.formatted_gps_geolocation.oui                         = 0xFFFFFFFF;
    p_.if_context.formatted_gps_geolocation.integer_second_timestamp    = 0xABABBABA;
    p_.if_context.formatted_gps_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    p_.if_context.formatted_gps_geolocation.latitude                    = -1.0;
    p_.if_context.formatted_gps_geolocation.longitude                   = -1.0;
    p_.if_context.formatted_gps_geolocation.altitude                    = -1.0;
    p_.if_context.formatted_gps_geolocation.speed_over_ground           = -1.0;
    p_.if_context.formatted_gps_geolocation.heading_angle               = -1.0;
    p_.if_context.formatted_gps_geolocation.track_angle                 = -1.0;
    p_.if_context.formatted_gps_geolocation.magnetic_variation          = -1.0;

    p_.if_context.formatted_ins_geolocation.tsi                         = VRT_TSI_OTHER;
    p_.if_context.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.formatted_ins_geolocation.oui                         = 0xFFFFFFFF;
    p_.if_context.formatted_ins_geolocation.integer_second_timestamp    = 0xABABBABA;
    p_.if_context.formatted_ins_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    p_.if_context.formatted_ins_geolocation.latitude                    = -1.0;
    p_.if_context.formatted_ins_geolocation.longitude                   = -1.0;
    p_.if_context.formatted_ins_geolocation.altitude                    = -1.0;
    p_.if_context.formatted_ins_geolocation.speed_over_ground           = -1.0;
    p_.if_context.formatted_ins_geolocation.heading_angle               = -1.0;
    p_.if_context.formatted_ins_geolocation.track_angle                 = -1.0;
    p_.if_context.formatted_ins_geolocation.magnetic_variation          = -1.0;

    p_.if_context.ecef_ephemeris.tsi                         = VRT_TSI_OTHER;
    p_.if_context.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.ecef_ephemeris.oui                         = 0xFFFFFFFF;
    p_.if_context.ecef_ephemeris.integer_second_timestamp    = 0xABABBABA;
    p_.if_context.ecef_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    p_.if_context.ecef_ephemeris.position_x                  = -1.0;
    p_.if_context.ecef_ephemeris.position_y                  = -1.0;
    p_.if_context.ecef_ephemeris.position_z                  = -1.0;
    p_.if_context.ecef_ephemeris.attitude_alpha              = -1.0;
    p_.if_context.ecef_ephemeris.attitude_beta               = -1.0;
    p_.if_context.ecef_ephemeris.attitude_phi                = -1.0;
    p_.if_context.ecef_ephemeris.velocity_dx                 = -1.0;
    p_.if_context.ecef_ephemeris.velocity_dy                 = -1.0;
    p_.if_context.ecef_ephemeris.velocity_dz                 = -1.0;

    p_.if_context.relative_ephemeris.tsi                         = VRT_TSI_OTHER;
    p_.if_context.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    p_.if_context.relative_ephemeris.oui                         = 0xFFFFFFFF;
    p_.if_context.relative_ephemeris.integer_second_timestamp    = 0xABABBABA;
    p_.if_context.relative_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    p_.if_context.relative_ephemeris.position_x                  = -1.0;
    p_.if_context.relative_ephemeris.position_y                  = -1.0;
    p_.if_context.relative_ephemeris.position_z                  = -1.0;
    p_.if_context.relative_ephemeris.attitude_alpha              = -1.0;
    p_.if_context.relative_ephemeris.attitude_beta               = -1.0;
    p_.if_context.relative_ephemeris.attitude_phi                = -1.0;
    p_.if_context.relative_ephemeris.velocity_dx                 = -1.0;
    p_.if_context.relative_ephemeris.velocity_dy                 = -1.0;
    p_.if_context.relative_ephemeris.velocity_dz                 = -1.0;

    p_.if_context.ephemeris_reference_identifier = 0xDEDEDEDE;

    p_.if_context.gps_ascii.oui             = 0xFFFFFFFF;
    p_.if_context.gps_ascii.number_of_words = 3;
    p_.if_context.gps_ascii.ascii           = "Lorem ipsum!";

    std::array<uint32_t, 2> l1                                                            = {0x12345678, 0x23456789};
    std::array<uint32_t, 2> l2                                                            = {0x3456789A, 0x456789AB};
    std::array<uint32_t, 2> l3                                                            = {0x56789ABC, 0x6789ABCD};
    std::array<uint32_t, 2> l4                                                            = {0x789ABCDE, 0x89ABCDEF};
    std::array<uint32_t, 2> l5                                                            = {0x9ABCDEF0, 0xABCDEF01};
    p_.if_context.context_association_lists.has.asynchronous_channel_tag_list             = true;
    p_.if_context.context_association_lists.source_list_size                              = l1.size();
    p_.if_context.context_association_lists.system_list_size                              = l2.size();
    p_.if_context.context_association_lists.vector_component_list_size                    = l3.size();
    p_.if_context.context_association_lists.asynchronous_channel_list_size                = l4.size();
    p_.if_context.context_association_lists.source_context_association_list               = l1.data();
    p_.if_context.context_association_lists.system_context_association_list               = l2.data();
    p_.if_context.context_association_lists.vector_component_context_association_list     = l3.data();
    p_.if_context.context_association_lists.asynchronous_channel_context_association_list = l4.data();
    p_.if_context.context_association_lists.asynchronous_channel_tag_list                 = l5.data();
}
