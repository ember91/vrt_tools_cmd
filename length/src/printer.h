#ifndef VRT_LENGTH_SRC_PRINTER_H_
#define VRT_LENGTH_SRC_PRINTER_H_

namespace vrt::common {
class StreamHistory;
}
namespace vrt::common {
struct PacketIdDiffs;
}

namespace vrt::length {

void print_difference(const common::StreamHistory& stream_history, const common::PacketIdDiffs& differences);

}  // namespace vrt::length

#endif
