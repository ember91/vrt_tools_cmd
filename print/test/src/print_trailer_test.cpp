#include <gtest/gtest.h>

#include <array>
#include <cstdint>

#include <vrt/vrt_init.h>
#include <vrt/vrt_write.h>

#include "../../src/type_printer.h"

class PrintTrailerTest : public ::testing::Test {
   protected:
    PrintTrailerTest() : t_() {}

    void SetUp() override { vrt_init_trailer(&t_); }

    void TearDown() override { vrt::print::print_trailer(t_); }

    vrt_trailer t_;
};

TEST_F(PrintTrailerTest, None) {}

TEST_F(PrintTrailerTest, HasCalibratedTime) {
    t_.has.calibrated_time = true;
}

TEST_F(PrintTrailerTest, HasValidData) {
    t_.has.valid_data = true;
}

TEST_F(PrintTrailerTest, HasReferenceLock) {
    t_.has.reference_lock = true;
}

TEST_F(PrintTrailerTest, HasAgcOrMgc) {
    t_.has.agc_or_mgc = true;
}

TEST_F(PrintTrailerTest, HasDetectedSignal) {
    t_.has.detected_signal = true;
}

TEST_F(PrintTrailerTest, HasSpectralInversion) {
    t_.has.spectral_inversion = true;
}

TEST_F(PrintTrailerTest, HasOverRange) {
    t_.has.over_range = true;
}

TEST_F(PrintTrailerTest, HasSampleLoss) {
    t_.has.sample_loss = true;
}

TEST_F(PrintTrailerTest, HasUserDefined11) {
    t_.has.user_defined11 = true;
}

TEST_F(PrintTrailerTest, HasUserDefined10) {
    t_.has.user_defined10 = true;
}

TEST_F(PrintTrailerTest, HasUserDefined9) {
    t_.has.user_defined9 = true;
}

TEST_F(PrintTrailerTest, HasUserDefined8) {
    t_.has.user_defined8 = true;
}

TEST_F(PrintTrailerTest, CalibratedTime) {
    t_.calibrated_time = true;
}

TEST_F(PrintTrailerTest, ValidData) {
    t_.valid_data = true;
}

TEST_F(PrintTrailerTest, ReferenceLock) {
    t_.reference_lock = true;
}

TEST_F(PrintTrailerTest, AgcOrMgc) {
    t_.agc_or_mgc = VRT_AOM_AGC;
}

TEST_F(PrintTrailerTest, DetectedSignal) {
    t_.detected_signal = true;
}

TEST_F(PrintTrailerTest, SpectralInversion) {
    t_.spectral_inversion = true;
}

TEST_F(PrintTrailerTest, OverRange) {
    t_.over_range = true;
}

TEST_F(PrintTrailerTest, SampleLoss) {
    t_.sample_loss = true;
}

TEST_F(PrintTrailerTest, UserDefined11) {
    t_.user_defined11 = true;
}

TEST_F(PrintTrailerTest, UserDefined10) {
    t_.user_defined10 = true;
}

TEST_F(PrintTrailerTest, UserDefined9) {
    t_.user_defined9 = true;
}

TEST_F(PrintTrailerTest, UserDefined8) {
    t_.user_defined8 = true;
}

TEST_F(PrintTrailerTest, HasAssociatedPacketCount) {
    t_.has.associated_context_packet_count = true;
}

TEST_F(PrintTrailerTest, AssociatedPacketCount) {
    t_.associated_context_packet_count = 0xFF;
}

TEST_F(PrintTrailerTest, BothCalibratedTime) {
    t_.has.calibrated_time = true;
    t_.calibrated_time     = true;
}

TEST_F(PrintTrailerTest, BothValidData) {
    t_.has.valid_data = true;
    t_.valid_data     = true;
}

TEST_F(PrintTrailerTest, BothReferenceLock) {
    t_.has.reference_lock = true;
    t_.reference_lock     = true;
}

TEST_F(PrintTrailerTest, BothAgcOrMgc) {
    t_.has.agc_or_mgc = true;
    t_.agc_or_mgc     = VRT_AOM_AGC;
}

TEST_F(PrintTrailerTest, BothDetectedSignal) {
    t_.has.detected_signal = true;
    t_.detected_signal     = true;
}

TEST_F(PrintTrailerTest, BothSpectralInversion) {
    t_.has.spectral_inversion = true;
    t_.spectral_inversion     = true;
}

TEST_F(PrintTrailerTest, BothOverRange) {
    t_.has.over_range = true;
    t_.over_range     = true;
}

TEST_F(PrintTrailerTest, BothSampleLoss) {
    t_.has.sample_loss = true;
    t_.sample_loss     = true;
}

TEST_F(PrintTrailerTest, BothUserDefined11) {
    t_.has.user_defined11 = true;
    t_.user_defined11     = true;
}

TEST_F(PrintTrailerTest, BothUserDefined10) {
    t_.has.user_defined10 = true;
    t_.user_defined10     = true;
}

TEST_F(PrintTrailerTest, BothUserDefined9) {
    t_.has.user_defined9 = true;
    t_.user_defined9     = true;
}

TEST_F(PrintTrailerTest, BothUserDefined8) {
    t_.has.user_defined8 = true;
    t_.user_defined8     = true;
}

TEST_F(PrintTrailerTest, BothAssociatedContextPacketCount) {
    t_.has.associated_context_packet_count = true;
    t_.associated_context_packet_count     = 0x7E;
}

TEST_F(PrintTrailerTest, EveryOther1) {
    t_.has.calibrated_time                 = true;
    t_.has.reference_lock                  = true;
    t_.has.detected_signal                 = true;
    t_.has.over_range                      = true;
    t_.has.user_defined11                  = true;
    t_.has.user_defined9                   = true;
    t_.calibrated_time                     = true;
    t_.reference_lock                      = true;
    t_.detected_signal                     = true;
    t_.over_range                          = true;
    t_.user_defined11                      = true;
    t_.user_defined9                       = true;
    t_.has.associated_context_packet_count = true;
}

TEST_F(PrintTrailerTest, EveryOther2) {
    t_.has.valid_data                  = true;
    t_.has.agc_or_mgc                  = true;
    t_.has.spectral_inversion          = true;
    t_.has.sample_loss                 = true;
    t_.has.user_defined10              = true;
    t_.has.user_defined8               = true;
    t_.valid_data                      = true;
    t_.agc_or_mgc                      = VRT_AOM_AGC;
    t_.spectral_inversion              = true;
    t_.sample_loss                     = true;
    t_.user_defined10                  = true;
    t_.user_defined8                   = true;
    t_.associated_context_packet_count = 0xFF;
}

TEST_F(PrintTrailerTest, All) {
    t_.has.calibrated_time                 = true;
    t_.has.valid_data                      = true;
    t_.has.reference_lock                  = true;
    t_.has.agc_or_mgc                      = true;
    t_.has.detected_signal                 = true;
    t_.has.spectral_inversion              = true;
    t_.has.over_range                      = true;
    t_.has.sample_loss                     = true;
    t_.has.user_defined11                  = true;
    t_.has.user_defined10                  = true;
    t_.has.user_defined9                   = true;
    t_.has.user_defined8                   = true;
    t_.calibrated_time                     = true;
    t_.valid_data                          = true;
    t_.reference_lock                      = true;
    t_.agc_or_mgc                          = VRT_AOM_AGC;
    t_.detected_signal                     = true;
    t_.spectral_inversion                  = true;
    t_.over_range                          = true;
    t_.sample_loss                         = true;
    t_.user_defined11                      = true;
    t_.user_defined10                      = true;
    t_.user_defined9                       = true;
    t_.user_defined8                       = true;
    t_.has.associated_context_packet_count = true;
    t_.associated_context_packet_count     = 0xFF;
}
