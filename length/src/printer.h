#ifndef VRT_LENGTH_SRC_PRINTER_H_
#define VRT_LENGTH_SRC_PRINTER_H_

#include "packet_id_differences.h"
#include "stream_history.h"

namespace vrt::length {

void print_difference(const StreamHistory& stream_history, const PacketIdDiffs& differences);

}  // namespace vrt::length

#endif
