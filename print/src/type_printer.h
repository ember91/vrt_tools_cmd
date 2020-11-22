#ifndef VRT_PRINT_SRC_TYPE_PRINTER_H_
#define VRT_PRINT_SRC_TYPE_PRINTER_H_

#include <cstdint>

#include <vrt/vrt_types.h>

namespace vrt::print {

void print_header(const vrt_header& header);
void print_fields(const vrt_header& header, const vrt_fields& fields);
void print_body(const vrt_header& header, int32_t words_body);
void print_if_context(const vrt_if_context& if_context);
void print_trailer(const vrt_trailer& trailer);

}  // namespace vrt::print

#endif
