#include <gtest/gtest.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_types.h>

#include "../../src/type_printer.h"

class PrintTrailerTest : public ::testing::Test {
   protected:
    void SetUp() override { vrt_init_packet(&p_); }

    void TearDown() override { vrt::print::print_trailer(p_); }

    vrt_packet p_;
};

TEST_F(PrintTrailerTest, None) {}

TEST_F(PrintTrailerTest, HasCalibratedTime) {
    p_.trailer.has.calibrated_time = true;
}

TEST_F(PrintTrailerTest, HasValidData) {
    p_.trailer.has.valid_data = true;
}

TEST_F(PrintTrailerTest, HasReferenceLock) {
    p_.trailer.has.reference_lock = true;
}

TEST_F(PrintTrailerTest, HasAgcOrMgc) {
    p_.trailer.has.agc_or_mgc = true;
}

TEST_F(PrintTrailerTest, HasDetectedSignal) {
    p_.trailer.has.detected_signal = true;
}

TEST_F(PrintTrailerTest, HasSpectralInversion) {
    p_.trailer.has.spectral_inversion = true;
}

TEST_F(PrintTrailerTest, HasOverRange) {
    p_.trailer.has.over_range = true;
}

TEST_F(PrintTrailerTest, HasSampleLoss) {
    p_.trailer.has.sample_loss = true;
}

TEST_F(PrintTrailerTest, HasUserDefined11) {
    p_.trailer.has.user_defined11 = true;
}

TEST_F(PrintTrailerTest, HasUserDefined10) {
    p_.trailer.has.user_defined10 = true;
}

TEST_F(PrintTrailerTest, HasUserDefined9) {
    p_.trailer.has.user_defined9 = true;
}

TEST_F(PrintTrailerTest, HasUserDefined8) {
    p_.trailer.has.user_defined8 = true;
}

TEST_F(PrintTrailerTest, CalibratedTime) {
    p_.trailer.calibrated_time = true;
}

TEST_F(PrintTrailerTest, ValidData) {
    p_.trailer.valid_data = true;
}

TEST_F(PrintTrailerTest, ReferenceLock) {
    p_.trailer.reference_lock = true;
}

TEST_F(PrintTrailerTest, AgcOrMgc) {
    p_.trailer.agc_or_mgc = VRT_AOM_AGC;
}

TEST_F(PrintTrailerTest, DetectedSignal) {
    p_.trailer.detected_signal = true;
}

TEST_F(PrintTrailerTest, SpectralInversion) {
    p_.trailer.spectral_inversion = true;
}

TEST_F(PrintTrailerTest, OverRange) {
    p_.trailer.over_range = true;
}

TEST_F(PrintTrailerTest, SampleLoss) {
    p_.trailer.sample_loss = true;
}

TEST_F(PrintTrailerTest, UserDefined11) {
    p_.trailer.user_defined11 = true;
}

TEST_F(PrintTrailerTest, UserDefined10) {
    p_.trailer.user_defined10 = true;
}

TEST_F(PrintTrailerTest, UserDefined9) {
    p_.trailer.user_defined9 = true;
}

TEST_F(PrintTrailerTest, UserDefined8) {
    p_.trailer.user_defined8 = true;
}

TEST_F(PrintTrailerTest, HasAssociatedPacketCount) {
    p_.trailer.has.associated_context_packet_count = true;
}

TEST_F(PrintTrailerTest, AssociatedPacketCount) {
    p_.trailer.associated_context_packet_count = 0xFF;
}

TEST_F(PrintTrailerTest, BothCalibratedTime) {
    p_.trailer.has.calibrated_time = true;
    p_.trailer.calibrated_time     = true;
}

TEST_F(PrintTrailerTest, BothValidData) {
    p_.trailer.has.valid_data = true;
    p_.trailer.valid_data     = true;
}

TEST_F(PrintTrailerTest, BothReferenceLock) {
    p_.trailer.has.reference_lock = true;
    p_.trailer.reference_lock     = true;
}

TEST_F(PrintTrailerTest, BothAgcOrMgc) {
    p_.trailer.has.agc_or_mgc = true;
    p_.trailer.agc_or_mgc     = VRT_AOM_AGC;
}

TEST_F(PrintTrailerTest, BothDetectedSignal) {
    p_.trailer.has.detected_signal = true;
    p_.trailer.detected_signal     = true;
}

TEST_F(PrintTrailerTest, BothSpectralInversion) {
    p_.trailer.has.spectral_inversion = true;
    p_.trailer.spectral_inversion     = true;
}

TEST_F(PrintTrailerTest, BothOverRange) {
    p_.trailer.has.over_range = true;
    p_.trailer.over_range     = true;
}

TEST_F(PrintTrailerTest, BothSampleLoss) {
    p_.trailer.has.sample_loss = true;
    p_.trailer.sample_loss     = true;
}

TEST_F(PrintTrailerTest, BothUserDefined11) {
    p_.trailer.has.user_defined11 = true;
    p_.trailer.user_defined11     = true;
}

TEST_F(PrintTrailerTest, BothUserDefined10) {
    p_.trailer.has.user_defined10 = true;
    p_.trailer.user_defined10     = true;
}

TEST_F(PrintTrailerTest, BothUserDefined9) {
    p_.trailer.has.user_defined9 = true;
    p_.trailer.user_defined9     = true;
}

TEST_F(PrintTrailerTest, BothUserDefined8) {
    p_.trailer.has.user_defined8 = true;
    p_.trailer.user_defined8     = true;
}

TEST_F(PrintTrailerTest, BothAssociatedContextPacketCount) {
    p_.trailer.has.associated_context_packet_count = true;
    p_.trailer.associated_context_packet_count     = 0x7E;
}

TEST_F(PrintTrailerTest, EveryOther1) {
    p_.trailer.has.calibrated_time                 = true;
    p_.trailer.has.reference_lock                  = true;
    p_.trailer.has.detected_signal                 = true;
    p_.trailer.has.over_range                      = true;
    p_.trailer.has.user_defined11                  = true;
    p_.trailer.has.user_defined9                   = true;
    p_.trailer.calibrated_time                     = true;
    p_.trailer.reference_lock                      = true;
    p_.trailer.detected_signal                     = true;
    p_.trailer.over_range                          = true;
    p_.trailer.user_defined11                      = true;
    p_.trailer.user_defined9                       = true;
    p_.trailer.has.associated_context_packet_count = true;
}

TEST_F(PrintTrailerTest, EveryOther2) {
    p_.trailer.has.valid_data                  = true;
    p_.trailer.has.agc_or_mgc                  = true;
    p_.trailer.has.spectral_inversion          = true;
    p_.trailer.has.sample_loss                 = true;
    p_.trailer.has.user_defined10              = true;
    p_.trailer.has.user_defined8               = true;
    p_.trailer.valid_data                      = true;
    p_.trailer.agc_or_mgc                      = VRT_AOM_AGC;
    p_.trailer.spectral_inversion              = true;
    p_.trailer.sample_loss                     = true;
    p_.trailer.user_defined10                  = true;
    p_.trailer.user_defined8                   = true;
    p_.trailer.associated_context_packet_count = 0xFF;
}

TEST_F(PrintTrailerTest, All) {
    p_.trailer.has.calibrated_time                 = true;
    p_.trailer.has.valid_data                      = true;
    p_.trailer.has.reference_lock                  = true;
    p_.trailer.has.agc_or_mgc                      = true;
    p_.trailer.has.detected_signal                 = true;
    p_.trailer.has.spectral_inversion              = true;
    p_.trailer.has.over_range                      = true;
    p_.trailer.has.sample_loss                     = true;
    p_.trailer.has.user_defined11                  = true;
    p_.trailer.has.user_defined10                  = true;
    p_.trailer.has.user_defined9                   = true;
    p_.trailer.has.user_defined8                   = true;
    p_.trailer.calibrated_time                     = true;
    p_.trailer.valid_data                          = true;
    p_.trailer.reference_lock                      = true;
    p_.trailer.agc_or_mgc                          = VRT_AOM_AGC;
    p_.trailer.detected_signal                     = true;
    p_.trailer.spectral_inversion                  = true;
    p_.trailer.over_range                          = true;
    p_.trailer.sample_loss                         = true;
    p_.trailer.user_defined11                      = true;
    p_.trailer.user_defined10                      = true;
    p_.trailer.user_defined9                       = true;
    p_.trailer.user_defined8                       = true;
    p_.trailer.has.associated_context_packet_count = true;
    p_.trailer.associated_context_packet_count     = 0xFF;
}
