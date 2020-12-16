#ifndef LIB_COMMON_INCLUDE_COMMON_GENERATE_TONE_H_
#define LIB_COMMON_INCLUDE_COMMON_GENERATE_TONE_H_

#include <cstdint>
#include <vector>

namespace vrt::common {

std::vector<float> generate_tone(uint64_t n = 512, float fc = 10000.0F, float sr = 44100.0F);

}  // namespace vrt::common

#endif
