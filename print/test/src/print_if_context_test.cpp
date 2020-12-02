#include <gtest/gtest.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_write.h>

#include "../../src/type_printer.h"

class PrintIfContextTest : public ::testing::Test {
   protected:
    PrintIfContextTest() : c_() {}

    void SetUp() override { vrt_init_if_context(&c_); }

    void TearDown() override { vrt::print::print_if_context(c_); }

    vrt_if_context c_;
};

TEST_F(PrintIfContextTest, None) {}

TEST_F(PrintIfContextTest, ContextFieldChange) {
    c_.context_field_change_indicator = true;
}

TEST_F(PrintIfContextTest, ReferencePointIdentifier) {
    c_.has.reference_point_identifier = true;
    c_.reference_point_identifier     = 0xFEFEFEFE;
}

TEST_F(PrintIfContextTest, Bandwidth) {
    c_.has.bandwidth = true;
    c_.bandwidth     = 4097.0;
}

TEST_F(PrintIfContextTest, IfReferenceFrequency) {
    c_.has.if_reference_frequency = true;
    c_.if_reference_frequency     = 4097.0;
}

TEST_F(PrintIfContextTest, RfReferenceFrequency) {
    c_.has.rf_reference_frequency = true;
    c_.rf_reference_frequency     = 4097.0;
}

TEST_F(PrintIfContextTest, RfReferenceFrequencyOffset) {
    c_.has.rf_reference_frequency_offset = true;
    c_.rf_reference_frequency_offset     = 4097.0;
}

TEST_F(PrintIfContextTest, IfBandOffset) {
    c_.has.if_band_offset = true;
    c_.if_band_offset     = 4097.0;
}

TEST_F(PrintIfContextTest, ReferenceLevel) {
    c_.has.reference_level = true;
    c_.reference_level     = -1.0F;
}

TEST_F(PrintIfContextTest, Gain) {
    c_.has.gain    = true;
    c_.gain.stage1 = -1.0F;
    c_.gain.stage2 = 1.0F;
}

TEST_F(PrintIfContextTest, OverRangeCount) {
    c_.has.over_range_count = true;
    c_.over_range_count     = 0xFEFEFEFE;
}

TEST_F(PrintIfContextTest, SampleRate) {
    c_.has.sample_rate = true;
    c_.sample_rate     = 4097.0;
}

TEST_F(PrintIfContextTest, TimestampAdjustment) {
    c_.has.timestamp_adjustment = true;
    c_.timestamp_adjustment     = 0xABABABABBEBEBEBE;
}

TEST_F(PrintIfContextTest, TimestampCalibrationTime) {
    c_.has.timestamp_calibration_time = true;
    c_.timestamp_calibration_time     = 0xABABABAB;
}

TEST_F(PrintIfContextTest, Temperature) {
    c_.has.temperature = true;
    c_.temperature     = -1.0F;
}

TEST_F(PrintIfContextTest, DeviceIdentifier) {
    c_.has.device_identifier         = true;
    c_.device_identifier.oui         = 0xFFFFFFFF;
    c_.device_identifier.device_code = 0xABAF;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasCalibratedTime) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.calibrated_time = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasValidData) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.valid_data = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasReferenceLock) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.has.reference_lock = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasAgcOrMgc) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.agc_or_mgc = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasDetectedSignal) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.detected_signal = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasSpectralInversion) {
    c_.has.state_and_event_indicators                    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasOverRange) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.over_range = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsHasSampleLoss) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.has.sample_loss = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsCalibratedTime) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.calibrated_time = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsValidData) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.valid_data = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsReferenceLock) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.reference_lock = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsAgcOrMgc) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.agc_or_mgc = VRT_AOM_AGC;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsDetectedSignal) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.detected_signal = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsSpectralInversion) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.spectral_inversion = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsOverRange) {
    c_.has.state_and_event_indicators        = true;
    c_.state_and_event_indicators.over_range = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsSampleLoss) {
    c_.has.state_and_event_indicators         = true;
    c_.state_and_event_indicators.sample_loss = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothCalibratedTime) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.calibrated_time = true;
    c_.state_and_event_indicators.calibrated_time     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothValidData) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.valid_data = true;
    c_.state_and_event_indicators.valid_data     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothReferenceLock) {
    c_.has.state_and_event_indicators                = true;
    c_.state_and_event_indicators.has.reference_lock = true;
    c_.state_and_event_indicators.reference_lock     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothAgcOrMgc) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.agc_or_mgc = true;
    c_.state_and_event_indicators.agc_or_mgc     = VRT_AOM_AGC;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothDetectedSignal) {
    c_.has.state_and_event_indicators                 = true;
    c_.state_and_event_indicators.has.detected_signal = true;
    c_.state_and_event_indicators.detected_signal     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothSpectralInversion) {
    c_.has.state_and_event_indicators                    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothOverRange) {
    c_.has.state_and_event_indicators            = true;
    c_.state_and_event_indicators.has.over_range = true;
    c_.state_and_event_indicators.over_range     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothSampleLoss) {
    c_.has.state_and_event_indicators             = true;
    c_.state_and_event_indicators.has.sample_loss = true;
    c_.state_and_event_indicators.sample_loss     = true;
}

TEST_F(PrintIfContextTest, StateAndEventIndicatorsBothUserDefined) {
    c_.has.state_and_event_indicators          = true;
    c_.state_and_event_indicators.user_defined = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatPackingMethod) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.packing_method = VRT_PM_LINK_EFFICIENT;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatRealOrComplex1) {
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = VRT_ROC_COMPLEX_POLAR;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatRealOrComplex2) {
    c_.has.data_packet_payload_format             = true;
    c_.data_packet_payload_format.real_or_complex = static_cast<vrt_real_complex>(3);
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatDataItemFormat1) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatDataItemFormat2) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.data_item_format = static_cast<vrt_data_item_format>(31);
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatSampleComponentRepeat) {
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.sample_component_repeat = true;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatEventTagSize) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.event_tag_size = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatChannelTagSize) {
    c_.has.data_packet_payload_format              = true;
    c_.data_packet_payload_format.channel_tag_size = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatItemPackingFieldSize) {
    c_.has.data_packet_payload_format                     = true;
    c_.data_packet_payload_format.item_packing_field_size = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatDataItemSize) {
    c_.has.data_packet_payload_format            = true;
    c_.data_packet_payload_format.data_item_size = 0xFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatRepeatCount) {
    c_.has.data_packet_payload_format          = true;
    c_.data_packet_payload_format.repeat_count = 0xFFFF;
}

TEST_F(PrintIfContextTest, DataPacketPayloadFormatVectorSize) {
    c_.has.data_packet_payload_format         = true;
    c_.data_packet_payload_format.vector_size = 0xFFFF;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationTsi) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationTsf) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationOui) {
    c_.has.formatted_gps_geolocation = true;
    c_.formatted_gps_geolocation.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationIntegerSecondTimestamp) {
    c_.has.formatted_gps_geolocation                      = true;
    c_.formatted_gps_geolocation.tsi                      = VRT_TSI_UTC;
    c_.formatted_gps_geolocation.integer_second_timestamp = 0xABABABAB;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationFractionalSecondTimestamp) {
    c_.has.formatted_gps_geolocation                         = true;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xABABABABBABABABA;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationLatitude) {
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.latitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationLongitude) {
    c_.has.formatted_gps_geolocation       = true;
    c_.formatted_gps_geolocation.longitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationAltitude) {
    c_.has.formatted_gps_geolocation      = true;
    c_.formatted_gps_geolocation.altitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationSpeedOverGround) {
    c_.has.formatted_gps_geolocation               = true;
    c_.formatted_gps_geolocation.speed_over_ground = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationHeadingAngle) {
    c_.has.formatted_gps_geolocation           = true;
    c_.formatted_gps_geolocation.heading_angle = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationTrackAngle) {
    c_.has.formatted_gps_geolocation         = true;
    c_.formatted_gps_geolocation.track_angle = 1.0;
}

TEST_F(PrintIfContextTest, FormattedGpsGeolocationMagneticVariation) {
    c_.has.formatted_gps_geolocation                = true;
    c_.formatted_gps_geolocation.magnetic_variation = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationTsi) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationTsf) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationOui) {
    c_.has.formatted_ins_geolocation = true;
    c_.formatted_ins_geolocation.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationIntegerSecondTimestamp) {
    c_.has.formatted_ins_geolocation                      = true;
    c_.formatted_ins_geolocation.tsi                      = VRT_TSI_UTC;
    c_.formatted_ins_geolocation.integer_second_timestamp = 0xABABABAB;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationFractionalSecondTimestamp) {
    c_.has.formatted_ins_geolocation                         = true;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_REAL_TIME;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xABABABABBABABABA;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationLatitude) {
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.latitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationLongitude) {
    c_.has.formatted_ins_geolocation       = true;
    c_.formatted_ins_geolocation.longitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationAltitude) {
    c_.has.formatted_ins_geolocation      = true;
    c_.formatted_ins_geolocation.altitude = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationSpeedOverGround) {
    c_.has.formatted_ins_geolocation               = true;
    c_.formatted_ins_geolocation.speed_over_ground = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationHeadingAngle) {
    c_.has.formatted_ins_geolocation           = true;
    c_.formatted_ins_geolocation.heading_angle = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationTrackAngle) {
    c_.has.formatted_ins_geolocation         = true;
    c_.formatted_ins_geolocation.track_angle = 1.0;
}

TEST_F(PrintIfContextTest, FormattedInsGeolocationMagneticVariation) {
    c_.has.formatted_ins_geolocation                = true;
    c_.formatted_ins_geolocation.magnetic_variation = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisTsi) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintIfContextTest, EcefEphemerisTsf) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintIfContextTest, EcefEphemerisOui) {
    c_.has.ecef_ephemeris = true;
    c_.ecef_ephemeris.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, EcefEphemerisIntegerSecondTimestamp) {
    c_.has.ecef_ephemeris                      = true;
    c_.ecef_ephemeris.tsi                      = VRT_TSI_UTC;
    c_.ecef_ephemeris.integer_second_timestamp = 0xABABABAB;
}

TEST_F(PrintIfContextTest, EcefEphemerisFractionalSecondTimestamp) {
    c_.has.ecef_ephemeris                         = true;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xABABABABBABABABA;
}

TEST_F(PrintIfContextTest, EcefEphemerisPositionX) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_x = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisPositionY) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_y = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisPositionZ) {
    c_.has.ecef_ephemeris        = true;
    c_.ecef_ephemeris.position_z = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisAltitudeAlpha) {
    c_.has.ecef_ephemeris            = true;
    c_.ecef_ephemeris.attitude_alpha = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisAltitudeBeta) {
    c_.has.ecef_ephemeris           = true;
    c_.ecef_ephemeris.attitude_beta = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisAltitudePhi) {
    c_.has.ecef_ephemeris          = true;
    c_.ecef_ephemeris.attitude_phi = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisVelocityDx) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dx = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisVelocityDy) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dy = 1.0;
}

TEST_F(PrintIfContextTest, EcefEphemerisVelocityDz) {
    c_.has.ecef_ephemeris         = true;
    c_.ecef_ephemeris.velocity_dz = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisTsi) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsi = VRT_TSI_OTHER;
}

TEST_F(PrintIfContextTest, RelativeEphemerisTsf) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.tsf = VRT_TSF_FREE_RUNNING_COUNT;
}

TEST_F(PrintIfContextTest, RelativeEphemerisOui) {
    c_.has.relative_ephemeris = true;
    c_.relative_ephemeris.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, RelativeEphemerisIntegerSecondTimestamp) {
    c_.has.relative_ephemeris                      = true;
    c_.relative_ephemeris.tsi                      = VRT_TSI_UTC;
    c_.relative_ephemeris.integer_second_timestamp = 0xABABABAB;
}

TEST_F(PrintIfContextTest, RelativeEphemerisFractionalSecondTimestamp) {
    c_.has.relative_ephemeris                         = true;
    c_.relative_ephemeris.tsf                         = VRT_TSF_REAL_TIME;
    c_.relative_ephemeris.fractional_second_timestamp = 0xABABABABBABABABA;
}

TEST_F(PrintIfContextTest, RelativeEphemerisPositionX) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_x = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisPositionY) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_y = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisPositionZ) {
    c_.has.relative_ephemeris        = true;
    c_.relative_ephemeris.position_z = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisAltitudeAlpha) {
    c_.has.relative_ephemeris            = true;
    c_.relative_ephemeris.attitude_alpha = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisAltitudeBeta) {
    c_.has.relative_ephemeris           = true;
    c_.relative_ephemeris.attitude_beta = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisAltitudePhi) {
    c_.has.relative_ephemeris          = true;
    c_.relative_ephemeris.attitude_phi = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisVelocityDx) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dx = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisVelocityDy) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dy = 1.0;
}

TEST_F(PrintIfContextTest, RelativeEphemerisVelocityDz) {
    c_.has.relative_ephemeris         = true;
    c_.relative_ephemeris.velocity_dz = 1.0;
}

TEST_F(PrintIfContextTest, EphemerisReferenceIdentifier) {
    c_.has.ephemeris_reference_identifier = true;
    c_.ephemeris_reference_identifier     = 0xFEFEFEFE;
}

TEST_F(PrintIfContextTest, GpsAsciiOui) {
    c_.has.gps_ascii = true;
    c_.gps_ascii.oui = 0xFFFFFFFF;
}

TEST_F(PrintIfContextTest, GpsAsciiAscii) {
    c_.has.gps_ascii             = true;
    c_.gps_ascii.number_of_words = 5;
    c_.gps_ascii.ascii           = "What's your name?";
}

TEST_F(PrintIfContextTest, ContextAssociationListsSource) {
    c_.has.context_association_lists                             = true;
    c_.context_association_lists.source_list_size                = 6;
    std::array<uint32_t, 6> l                                    = {};
    l[0]                                                         = 0xABABABAB;
    l[1]                                                         = 0xBCBCBCBC;
    l[2]                                                         = 0xCDCDCDCD;
    l[3]                                                         = 0xDEDEDEDE;
    l[4]                                                         = 0xEFEFEFEF;
    l[5]                                                         = 0x0F0F0F0F;
    c_.context_association_lists.source_context_association_list = l.data();
}

TEST_F(PrintIfContextTest, ContextAssociationListsSystem) {
    c_.has.context_association_lists                             = true;
    c_.context_association_lists.system_list_size                = 5;
    std::array<uint32_t, 5> l                                    = {};
    l[0]                                                         = 0xABABABAB;
    l[1]                                                         = 0xBCBCBCBC;
    l[2]                                                         = 0xCDCDCDCD;
    l[3]                                                         = 0xDEDEDEDE;
    l[4]                                                         = 0xEFEFEFEF;
    c_.context_association_lists.system_context_association_list = l.data();
}

TEST_F(PrintIfContextTest, ContextAssociationListsVectorComponent) {
    c_.has.context_association_lists                                       = true;
    c_.context_association_lists.vector_component_list_size                = 4;
    std::array<uint32_t, 4> l                                              = {};
    l[0]                                                                   = 0xABABABAB;
    l[1]                                                                   = 0xBCBCBCBC;
    l[2]                                                                   = 0xCDCDCDCD;
    l[3]                                                                   = 0xDEDEDEDE;
    c_.context_association_lists.vector_component_context_association_list = l.data();
}

TEST_F(PrintIfContextTest, ContextAssociationListsAsynchronous) {
    c_.has.context_association_lists                                           = true;
    c_.context_association_lists.asynchronous_channel_list_size                = 3;
    std::array<uint32_t, 3> l                                                  = {};
    l[0]                                                                       = 0xABABABAB;
    l[1]                                                                       = 0xBCBCBCBC;
    l[2]                                                                       = 0xCDCDCDCD;
    c_.context_association_lists.asynchronous_channel_context_association_list = l.data();
}

TEST_F(PrintIfContextTest, ContextAssociationListsAsynchronousTag) {
    c_.has.context_association_lists                                           = true;
    c_.context_association_lists.asynchronous_channel_list_size                = 2;
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    std::array<uint32_t, 2> l1                                                 = {};
    l1[0]                                                                      = 0xABABABAB;
    l1[1]                                                                      = 0xBCBCBCBC;
    std::array<uint32_t, 2> l2                                                 = {};
    l2[0]                                                                      = 0xABABABAB;
    l2[1]                                                                      = 0xBCBCBCBC;
    c_.context_association_lists.asynchronous_channel_context_association_list = l1.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l2.data();
}

TEST_F(PrintIfContextTest, EveryOther1) {
    c_.context_field_change_indicator = true;
    c_.has.bandwidth                  = true;
    c_.has.rf_reference_frequency     = true;
    c_.has.if_band_offset             = true;
    c_.has.gain                       = true;
    c_.has.sample_rate                = true;
    c_.has.timestamp_calibration_time = true;
    c_.has.device_identifier          = true;
    c_.has.data_packet_payload_format = true;
    c_.has.formatted_ins_geolocation  = true;
    c_.has.relative_ephemeris         = true;
    c_.has.gps_ascii                  = true;

    c_.bandwidth                     = -1.0;
    c_.rf_reference_frequency        = -1.0;
    c_.if_band_offset                = -1.0;
    c_.gain.stage1                   = -1.0F;
    c_.gain.stage2                   = -1.0F;
    c_.sample_rate                   = -1.0;
    c_.timestamp_calibration_time    = 0xBABABABA;
    c_.device_identifier.oui         = 0xFEBEFEBE;
    c_.device_identifier.device_code = 0xBEBA;

    c_.data_packet_payload_format.packing_method          = VRT_PM_LINK_EFFICIENT;
    c_.data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    c_.data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    c_.data_packet_payload_format.sample_component_repeat = true;
    c_.data_packet_payload_format.event_tag_size          = 0xFF;
    c_.data_packet_payload_format.channel_tag_size        = 0xFF;
    c_.data_packet_payload_format.item_packing_field_size = 0xFF;
    c_.data_packet_payload_format.data_item_size          = 0xFF;
    c_.data_packet_payload_format.repeat_count            = 0xFFFF;
    c_.data_packet_payload_format.vector_size             = 0xFFFF;

    c_.formatted_ins_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_ins_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_ins_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_ins_geolocation.latitude                    = -1.0;
    c_.formatted_ins_geolocation.longitude                   = -1.0;
    c_.formatted_ins_geolocation.altitude                    = -1.0;
    c_.formatted_ins_geolocation.speed_over_ground           = -1.0;
    c_.formatted_ins_geolocation.heading_angle               = -1.0;
    c_.formatted_ins_geolocation.track_angle                 = -1.0;
    c_.formatted_ins_geolocation.magnetic_variation          = -1.0;

    c_.relative_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.relative_ephemeris.oui                         = 0xFFFFFFFF;
    c_.relative_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.relative_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.relative_ephemeris.position_x                  = -1.0;
    c_.relative_ephemeris.position_y                  = -1.0;
    c_.relative_ephemeris.position_z                  = -1.0;
    c_.relative_ephemeris.attitude_alpha              = -1.0;
    c_.relative_ephemeris.attitude_beta               = -1.0;
    c_.relative_ephemeris.attitude_phi                = -1.0;
    c_.relative_ephemeris.velocity_dx                 = -1.0;
    c_.relative_ephemeris.velocity_dy                 = -1.0;
    c_.relative_ephemeris.velocity_dz                 = -1.0;

    c_.gps_ascii.oui             = 0xFFFFFFFF;
    c_.gps_ascii.number_of_words = 3;
    c_.gps_ascii.ascii           = "Lorem ipsu";
}

TEST_F(PrintIfContextTest, EveryOther2) {
    c_.has.reference_point_identifier     = true;
    c_.has.if_reference_frequency         = true;
    c_.has.rf_reference_frequency_offset  = true;
    c_.has.reference_level                = true;
    c_.has.over_range_count               = true;
    c_.has.timestamp_adjustment           = true;
    c_.has.temperature                    = true;
    c_.has.state_and_event_indicators     = true;
    c_.has.formatted_gps_geolocation      = true;
    c_.has.ecef_ephemeris                 = true;
    c_.has.ephemeris_reference_identifier = true;
    c_.has.context_association_lists      = true;

    c_.reference_point_identifier    = 0xACACACAC;
    c_.if_reference_frequency        = -1.0;
    c_.rf_reference_frequency_offset = -1.0;
    c_.reference_level               = -1.0F;
    c_.over_range_count              = 0xBDBDBDBD;
    c_.timestamp_adjustment          = 0xABCABCABCABCABCA;
    c_.temperature                   = -1.0F;

    c_.state_and_event_indicators.has.calibrated_time    = true;
    c_.state_and_event_indicators.has.valid_data         = true;
    c_.state_and_event_indicators.has.reference_lock     = true;
    c_.state_and_event_indicators.has.agc_or_mgc         = true;
    c_.state_and_event_indicators.has.detected_signal    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.has.over_range         = true;
    c_.state_and_event_indicators.has.sample_loss        = true;
    c_.state_and_event_indicators.calibrated_time        = true;
    c_.state_and_event_indicators.valid_data             = true;
    c_.state_and_event_indicators.reference_lock         = true;
    c_.state_and_event_indicators.agc_or_mgc             = VRT_AOM_AGC;
    c_.state_and_event_indicators.detected_signal        = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
    c_.state_and_event_indicators.over_range             = true;
    c_.state_and_event_indicators.sample_loss            = true;
    c_.state_and_event_indicators.user_defined           = 0xFF;

    c_.formatted_gps_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_gps_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_gps_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_gps_geolocation.latitude                    = -1.0;
    c_.formatted_gps_geolocation.longitude                   = -1.0;
    c_.formatted_gps_geolocation.altitude                    = -1.0;
    c_.formatted_gps_geolocation.speed_over_ground           = -1.0;
    c_.formatted_gps_geolocation.heading_angle               = -1.0;
    c_.formatted_gps_geolocation.track_angle                 = -1.0;
    c_.formatted_gps_geolocation.magnetic_variation          = -1.0;

    c_.ecef_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.ecef_ephemeris.oui                         = 0xFFFFFFFF;
    c_.ecef_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.ecef_ephemeris.position_x                  = -1.0;
    c_.ecef_ephemeris.position_y                  = -1.0;
    c_.ecef_ephemeris.position_z                  = -1.0;
    c_.ecef_ephemeris.attitude_alpha              = -1.0;
    c_.ecef_ephemeris.attitude_beta               = -1.0;
    c_.ecef_ephemeris.attitude_phi                = -1.0;
    c_.ecef_ephemeris.velocity_dx                 = -1.0;
    c_.ecef_ephemeris.velocity_dy                 = -1.0;
    c_.ecef_ephemeris.velocity_dz                 = -1.0;

    c_.ephemeris_reference_identifier = 0xDEDEDEDE;

    std::array<uint32_t, 2> l1                                                 = {0x12345678, 0x23456789};
    std::array<uint32_t, 2> l2                                                 = {0x3456789A, 0x456789AB};
    std::array<uint32_t, 2> l3                                                 = {0x56789ABC, 0x6789ABCD};
    std::array<uint32_t, 2> l4                                                 = {0x789ABCDE, 0x89ABCDEF};
    std::array<uint32_t, 2> l5                                                 = {0x9ABCDEF0, 0xABCDEF01};
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    c_.context_association_lists.source_list_size                              = l1.size();
    c_.context_association_lists.system_list_size                              = l2.size();
    c_.context_association_lists.vector_component_list_size                    = l3.size();
    c_.context_association_lists.asynchronous_channel_list_size                = l4.size();
    c_.context_association_lists.source_context_association_list               = l1.data();
    c_.context_association_lists.system_context_association_list               = l2.data();
    c_.context_association_lists.vector_component_context_association_list     = l3.data();
    c_.context_association_lists.asynchronous_channel_context_association_list = l4.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l5.data();
}

TEST_F(PrintIfContextTest, All) {
    c_.context_field_change_indicator     = true;
    c_.has.reference_point_identifier     = true;
    c_.has.bandwidth                      = true;
    c_.has.if_reference_frequency         = true;
    c_.has.rf_reference_frequency         = true;
    c_.has.rf_reference_frequency_offset  = true;
    c_.has.if_band_offset                 = true;
    c_.has.reference_level                = true;
    c_.has.gain                           = true;
    c_.has.over_range_count               = true;
    c_.has.sample_rate                    = true;
    c_.has.timestamp_adjustment           = true;
    c_.has.timestamp_calibration_time     = true;
    c_.has.temperature                    = true;
    c_.has.device_identifier              = true;
    c_.has.state_and_event_indicators     = true;
    c_.has.data_packet_payload_format     = true;
    c_.has.formatted_gps_geolocation      = true;
    c_.has.formatted_ins_geolocation      = true;
    c_.has.ecef_ephemeris                 = true;
    c_.has.relative_ephemeris             = true;
    c_.has.ephemeris_reference_identifier = true;
    c_.has.gps_ascii                      = true;
    c_.has.context_association_lists      = true;

    c_.reference_point_identifier    = 0xACACACAC;
    c_.bandwidth                     = -1.0;
    c_.if_reference_frequency        = -1.0;
    c_.rf_reference_frequency        = -1.0;
    c_.rf_reference_frequency_offset = -1.0;
    c_.if_band_offset                = -1.0;
    c_.reference_level               = -1.0F;
    c_.gain.stage1                   = -1.0F;
    c_.gain.stage2                   = -1.0F;
    c_.over_range_count              = 0xBDBDBDBD;
    c_.sample_rate                   = -1.0;
    c_.timestamp_adjustment          = 0xABCABCABCABCABCA;
    c_.timestamp_calibration_time    = 0xBABABABA;
    c_.temperature                   = -1.0F;
    c_.device_identifier.oui         = 0xFEBEFEBE;
    c_.device_identifier.device_code = 0xBEBA;

    c_.state_and_event_indicators.has.calibrated_time    = true;
    c_.state_and_event_indicators.has.valid_data         = true;
    c_.state_and_event_indicators.has.reference_lock     = true;
    c_.state_and_event_indicators.has.agc_or_mgc         = true;
    c_.state_and_event_indicators.has.detected_signal    = true;
    c_.state_and_event_indicators.has.spectral_inversion = true;
    c_.state_and_event_indicators.has.over_range         = true;
    c_.state_and_event_indicators.has.sample_loss        = true;
    c_.state_and_event_indicators.calibrated_time        = true;
    c_.state_and_event_indicators.valid_data             = true;
    c_.state_and_event_indicators.reference_lock         = true;
    c_.state_and_event_indicators.agc_or_mgc             = VRT_AOM_AGC;
    c_.state_and_event_indicators.detected_signal        = true;
    c_.state_and_event_indicators.spectral_inversion     = true;
    c_.state_and_event_indicators.over_range             = true;
    c_.state_and_event_indicators.sample_loss            = true;
    c_.state_and_event_indicators.user_defined           = 0xFF;

    c_.data_packet_payload_format.packing_method          = VRT_PM_LINK_EFFICIENT;
    c_.data_packet_payload_format.real_or_complex         = VRT_ROC_COMPLEX_POLAR;
    c_.data_packet_payload_format.data_item_format        = VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT;
    c_.data_packet_payload_format.sample_component_repeat = true;
    c_.data_packet_payload_format.event_tag_size          = 0xFF;
    c_.data_packet_payload_format.channel_tag_size        = 0xFF;
    c_.data_packet_payload_format.item_packing_field_size = 0xFF;
    c_.data_packet_payload_format.data_item_size          = 0xFF;
    c_.data_packet_payload_format.repeat_count            = 0xFFFF;
    c_.data_packet_payload_format.vector_size             = 0xFFFF;

    c_.formatted_gps_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_gps_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_gps_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_gps_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_gps_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_gps_geolocation.latitude                    = -1.0;
    c_.formatted_gps_geolocation.longitude                   = -1.0;
    c_.formatted_gps_geolocation.altitude                    = -1.0;
    c_.formatted_gps_geolocation.speed_over_ground           = -1.0;
    c_.formatted_gps_geolocation.heading_angle               = -1.0;
    c_.formatted_gps_geolocation.track_angle                 = -1.0;
    c_.formatted_gps_geolocation.magnetic_variation          = -1.0;

    c_.formatted_ins_geolocation.tsi                         = VRT_TSI_OTHER;
    c_.formatted_ins_geolocation.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.formatted_ins_geolocation.oui                         = 0xFFFFFFFF;
    c_.formatted_ins_geolocation.integer_second_timestamp    = 0xABABBABA;
    c_.formatted_ins_geolocation.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.formatted_ins_geolocation.latitude                    = -1.0;
    c_.formatted_ins_geolocation.longitude                   = -1.0;
    c_.formatted_ins_geolocation.altitude                    = -1.0;
    c_.formatted_ins_geolocation.speed_over_ground           = -1.0;
    c_.formatted_ins_geolocation.heading_angle               = -1.0;
    c_.formatted_ins_geolocation.track_angle                 = -1.0;
    c_.formatted_ins_geolocation.magnetic_variation          = -1.0;

    c_.ecef_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.ecef_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.ecef_ephemeris.oui                         = 0xFFFFFFFF;
    c_.ecef_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.ecef_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.ecef_ephemeris.position_x                  = -1.0;
    c_.ecef_ephemeris.position_y                  = -1.0;
    c_.ecef_ephemeris.position_z                  = -1.0;
    c_.ecef_ephemeris.attitude_alpha              = -1.0;
    c_.ecef_ephemeris.attitude_beta               = -1.0;
    c_.ecef_ephemeris.attitude_phi                = -1.0;
    c_.ecef_ephemeris.velocity_dx                 = -1.0;
    c_.ecef_ephemeris.velocity_dy                 = -1.0;
    c_.ecef_ephemeris.velocity_dz                 = -1.0;

    c_.relative_ephemeris.tsi                         = VRT_TSI_OTHER;
    c_.relative_ephemeris.tsf                         = VRT_TSF_FREE_RUNNING_COUNT;
    c_.relative_ephemeris.oui                         = 0xFFFFFFFF;
    c_.relative_ephemeris.integer_second_timestamp    = 0xABABBABA;
    c_.relative_ephemeris.fractional_second_timestamp = 0xBEBEBABACECECBCB;
    c_.relative_ephemeris.position_x                  = -1.0;
    c_.relative_ephemeris.position_y                  = -1.0;
    c_.relative_ephemeris.position_z                  = -1.0;
    c_.relative_ephemeris.attitude_alpha              = -1.0;
    c_.relative_ephemeris.attitude_beta               = -1.0;
    c_.relative_ephemeris.attitude_phi                = -1.0;
    c_.relative_ephemeris.velocity_dx                 = -1.0;
    c_.relative_ephemeris.velocity_dy                 = -1.0;
    c_.relative_ephemeris.velocity_dz                 = -1.0;

    c_.ephemeris_reference_identifier = 0xDEDEDEDE;

    c_.gps_ascii.oui             = 0xFFFFFFFF;
    c_.gps_ascii.number_of_words = 3;
    c_.gps_ascii.ascii           = "Lorem ipsum!";

    std::array<uint32_t, 2> l1                                                 = {0x12345678, 0x23456789};
    std::array<uint32_t, 2> l2                                                 = {0x3456789A, 0x456789AB};
    std::array<uint32_t, 2> l3                                                 = {0x56789ABC, 0x6789ABCD};
    std::array<uint32_t, 2> l4                                                 = {0x789ABCDE, 0x89ABCDEF};
    std::array<uint32_t, 2> l5                                                 = {0x9ABCDEF0, 0xABCDEF01};
    c_.context_association_lists.has.asynchronous_channel_tag_list             = true;
    c_.context_association_lists.source_list_size                              = l1.size();
    c_.context_association_lists.system_list_size                              = l2.size();
    c_.context_association_lists.vector_component_list_size                    = l3.size();
    c_.context_association_lists.asynchronous_channel_list_size                = l4.size();
    c_.context_association_lists.source_context_association_list               = l1.data();
    c_.context_association_lists.system_context_association_list               = l2.data();
    c_.context_association_lists.vector_component_context_association_list     = l3.data();
    c_.context_association_lists.asynchronous_channel_context_association_list = l4.data();
    c_.context_association_lists.asynchronous_channel_tag_list                 = l5.data();
}
