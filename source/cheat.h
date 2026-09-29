#ifndef CHEAT_H
#define CHEAT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    const uint16_t *seq;
    uint8_t len;
    uint8_t pos;
} Cheat;

void cheat_init(Cheat *c, const uint16_t *seq, uint8_t len);

bool cheat_feed(Cheat *c, uint16_t pressed);

#endif
