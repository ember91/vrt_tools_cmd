#ifndef VRT_PRINT_SRC_STRINGIFY_H_
#define VRT_PRINT_SRC_STRINGIFY_H_

#include <cstdint>
#include <string>

namespace vrt::print {

/**
 * Convert bool to string representation.
 *
 * \param b Bool to convert.
 * \return String representation.
 */
inline std::string BoolToStr(bool b) {
    return b ? "true" : "false";
}

std::string Uint32ToHexStr(uint32_t v, int symbols = 8);

void WriteCols(const std::string& name, const std::string& value, unsigned indentation_level = 0);

}  // namespace vrt::print

#endif
