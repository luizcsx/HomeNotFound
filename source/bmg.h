#ifndef BMG_H
#define BMG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    const uint8_t *inf_entries;
    const uint8_t *dat;
    uint32_t dat_size;
    uint16_t count;
    uint16_t entry_size;
} Bmg;

bool bmg_load(Bmg *bmg, const void *data, uint32_t size);

uint16_t bmg_count(const Bmg *bmg);

const char *bmg_get(const Bmg *bmg, uint16_t index);

size_t bmg_to_ascii(const char *utf8, char *out, size_t out_size);

#endif
