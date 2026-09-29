#include "cheat.h"

void cheat_init(Cheat *c, const uint16_t *seq, uint8_t len)
{
    c->seq = seq;
    c->len = len;
    c->pos = 0;
}

bool cheat_feed(Cheat *c, uint16_t pressed)
{
    if (pressed == 0)
        return false;

    if (pressed == c->seq[c->pos])
    {
        c->pos++;
        if (c->pos == c->len)
        {
            c->pos = 0;
            return true;
        }
    }
    else
    {
        uint8_t k;
        for (k = c->pos; k > 0; k--)
        {
            bool match = (c->seq[k - 1] == pressed);
            for (uint8_t i = 0; i + 1 < k && match; i++)
            {
                if (c->seq[i] != c->seq[c->pos - (k - 1) + i])
                    match = false;
            }
            if (match)
                break;
        }
        c->pos = k;
    }

    return false;
}
