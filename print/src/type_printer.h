#ifndef VRT_PRINT_SRC_TYPE_PRINTER_H_
#define VRT_PRINT_SRC_TYPE_PRINTER_H_

#include <cstdint>

struct vrt_packet;

namespace vrt::print {

void print_header(const vrt_packet& packet);
void print_fields(const vrt_packet& packet, double sample_rate);
void print_body(const vrt_packet& packet);
void print_if_context(const vrt_packet& packet);
void print_trailer(const vrt_packet& packet);

}  // namespace vrt::print

#endif
