#include "stringify.h"

#include <iomanip>
#include <iostream>

namespace vrt::print {

/**
 * Convert uint32_t to a hexadecimal representation string.
 *
 * \param v       Value to convert.
 * \param symbols Number of hexadecimal symbols to use for the representation. Defaults to 8 (all).
 *
 * \return String representation.
 */
std::string Uint32ToHexStr(uint32_t v, int symbols) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setw(symbols) << std::setfill('0') << std::uppercase << v;
    return ss.str();
}

/**
 * Write name - value pair in a nice table format.
 *
 * \param name              Name to write in the left column.
 * \param value             Value to write in the right column.
 * \param indentation_level Indentation level to use for the left column. Defaults to 0 (none).
 */
void WriteCols(const std::string& name, const std::string& value, unsigned indentation_level) {
    // Number of spaces per indentation level
    const int INDENTATION_SPACES{2};

    // Total width in command line characters
    const int TOTAL_WIDTH{80};

    std::string indented_name{std::string(INDENTATION_SPACES * indentation_level, ' ') + name};
    int         width_left{static_cast<int>(indented_name.size())};
    int         width_right{static_cast<int>(TOTAL_WIDTH - width_left)};
    std::cout << std::left << std::setw(width_left) << indented_name << std::right << std::setw(width_right) << value
              << "\n";
}

}  // namespace vrt::print
