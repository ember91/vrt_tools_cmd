#ifndef VRT_LENGTH_SRC_PRINTER_H_
#define VRT_LENGTH_SRC_PRINTER_H_

namespace vrt::length {
class StreamHistory;
}
namespace vrt {
struct PacketIdDiffs;
}

namespace vrt::length {

void print_difference(const StreamHistory& stream_history, const PacketIdDiffs& differences);

}  // namespace vrt::length

#endif
