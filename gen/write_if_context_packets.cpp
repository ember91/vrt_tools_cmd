#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vrt/vrt_init.h>
#include <vrt/vrt_string.h>
#include <vrt/vrt_words.h>
#include <vrt/vrt_write.h>

/* Buffer size in 32-bit words */
#define SIZE 515

int main() {
    /* Packet data buffer */
    uint32_t b[SIZE];

    /* Initialize to reasonable values */
    vrt_header     h;
    vrt_fields     f;
    vrt_if_context c;
    vrt_init_header(&h);
    vrt_init_fields(&f);
    vrt_init_if_context(&c);

    /* Configure */
    h.packet_type     = VRT_PT_IF_CONTEXT;
    f.stream_id       = 0xDEADBEEF;
    c.has.bandwidth   = true;
    c.bandwidth       = 45.0;
    c.has.sample_rate = true;
    c.sample_rate     = 236.0;

    h.packet_size = VRT_WORDS_HEADER + vrt_words_fields(&h) + vrt_words_if_context(&c);

    /* Write header */
    int32_t offset{0};
    int32_t rv{vrt_write_header(&h, b + offset, SIZE - offset, true)};
    if (rv < 0) {
        fprintf(stderr, "Failed to write header: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write fields */
    rv = vrt_write_fields(&h, &f, b + offset, SIZE - offset, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write fields section: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;

    /* Write context */
    rv = vrt_write_if_context(&c, b + offset, SIZE - offset, true);
    if (rv < 0) {
        fprintf(stderr, "Failed to write context: %s\n", vrt_string_error(rv));
        return EXIT_FAILURE;
    }
    offset += rv;
    int32_t size{offset};

    /* Write generated packet to file */
    FILE* fp{fopen("if_context_100.vrt", "wb")};
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return EXIT_FAILURE;
    }
    for (int i{0}; i < 100; ++i) {
        if (fwrite(b, sizeof(uint32_t) * size, 1, fp) != 1) {
            fprintf(stderr, "Failed to write to file\n");
            fclose(fp);
            return EXIT_FAILURE;
        }
    }

    fclose(fp);

    return EXIT_SUCCESS;
}
