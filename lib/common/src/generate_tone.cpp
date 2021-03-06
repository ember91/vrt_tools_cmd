#include "common/generate_tone.h"

#include <cmath>
#include <cstddef>
#include <vector>

namespace vrt::common {

/**
 * Generate tone signal in vector.
 *
 * \param n  Number of elements to generate.
 * \param fc Center frequency [Hz].
 * \param sr Sample rate [Hz].
 *
 * \return Generated signal tone.
 */
std::vector<float> generate_tone(uint64_t n, float fc, float sr) {
    // M_PI is nonstandard :(
    const float PI{3.1415926F};

    std::vector<float> v(n);
    for (size_t i{0}; i < v.size(); ++i) {
        v[i] = std::sin(2.0F * PI * fc * static_cast<float>(i) / sr);
    }
    return v;
}

}  // namespace vrt::common
