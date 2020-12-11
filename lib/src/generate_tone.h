#ifndef VRT_TOOLS_CMD_LIB_GENERATE_TONE_H_
#define VRT_TOOLS_CMD_LIB_GENERATE_TONE_H_

#include <vector>

namespace vrt {

std::vector<float> generate_tone(size_t n = 512, float fc = 10000.0F, float sr = 44100.0F);

}  // namespace vrt

#endif
