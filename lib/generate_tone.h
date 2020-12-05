#ifndef VRT_TOOLS_CMD_LIB_GENERATE_TONE_H_
#define VRT_TOOLS_CMD_LIB_GENERATE_TONE_H_

#include <cmath>
#include <fstream>
#include <functional>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_types.h>
#include <vrt/vrt_words.h>
#include <vrt/vrt_write.h>

namespace vrt {

/**
 * Generate tone signal in vector.
 *
 * \param n  Number of elements to generate.
 * \param fc Center frequency [Hz].
 * \param sr Sample rate [Hz].
 *
 * \return Generated signal tone.
 */
std::vector<float> generate_tone(size_t n = 512, float fc = 10000.0F, float sr = 44100.0F) {
    // M_PI is nonstandard :(
    const float PI{3.1415926F};

    std::vector<float> v(n);
    for (int i{0}; i < v.size(); ++i) {
        v[i] = std::sin(2.0F * PI * fc * static_cast<float>(i) / sr);
    }
    return v;
}

}  // namespace vrt

#endif
