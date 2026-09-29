#include "bmg.h"

#include <string.h>

#define BMG_ENCODING_UTF8 4

static uint16_t be16(const uint8_t *p)
{
    return (uint16_t)((p[0] << 8) | p[1]);
}

static uint32_t be32(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

bool bmg_load(Bmg *bmg, const void *data, uint32_t size)
{
    const uint8_t *p = (const uint8_t *)data;

    if (bmg == NULL || p == NULL || size < 0x20)
        return false;
    if (memcmp(p, "MESGbmg1", 8) != 0)
        return false;
    if (p[0x10] != BMG_ENCODING_UTF8)
        return false;

    uint32_t file_size = be32(p + 0x08);
    uint32_t num_sections = be32(p + 0x0C);
    if (file_size > size)
        return false;

    bool have_inf = false;
    bool have_dat = false;
    Bmg tmp = { 0 };

    uint32_t off = 0x20;
    for (uint32_t i = 0; i < num_sections; i++)
    {
        if (off + 8 > file_size)
            return false;

        uint32_t sec_size = be32(p + off + 4);
        if (sec_size < 8 || off + sec_size > file_size)
            return false;

        if (memcmp(p + off, "INF1", 4) == 0)
        {
            if (sec_size < 0x10)
                return false;
            tmp.count = be16(p + off + 0x08);
            tmp.entry_size = be16(p + off + 0x0A);
            if (tmp.entry_size < 4)
                return false;
            if (0x10 + (uint32_t)tmp.count * tmp.entry_size > sec_size)
                return false;
            tmp.inf_entries = p + off + 0x10;
            have_inf = true;
        }
        else if (memcmp(p + off, "DAT1", 4) == 0)
        {
            tmp.dat = p + off + 8;
            tmp.dat_size = sec_size - 8;
            have_dat = true;
        }

        off += sec_size;
    }

    if (!have_inf || !have_dat)
        return false;

    *bmg = tmp;
    return true;
}

uint16_t bmg_count(const Bmg *bmg)
{
    return bmg->count;
}

const char *bmg_get(const Bmg *bmg, uint16_t index)
{
    if (index >= bmg->count)
        return NULL;

    uint32_t offset = be32(bmg->inf_entries + (uint32_t)index * bmg->entry_size);
    if (offset >= bmg->dat_size)
        return NULL;

    if (memchr(bmg->dat + offset, 0, bmg->dat_size - offset) == NULL)
        return NULL;

    return (const char *)(bmg->dat + offset);
}

static const char latin1_base[65] =
    "AAAAAAACEEEEIIII"
    "DNOOOOOxOUUUUYTs"
    "aaaaaaaceeeeiiii"
    "dnooooo/ouuuuyty";

size_t bmg_to_ascii(const char *utf8, char *out, size_t out_size)
{
    size_t n = 0;

    if (out_size == 0)
        return 0;

    while (*utf8 != '\0' && n + 1 < out_size)
    {
        uint8_t c = (uint8_t)*utf8;

        if (c < 0x80)
        {
            out[n++] = (char)c;
            utf8++;
        }
        else if (c == 0xC3 && ((uint8_t)utf8[1] & 0xC0) == 0x80)
        {
            out[n++] = latin1_base[(uint8_t)utf8[1] - 0x80];
            utf8 += 2;
        }
        else
        {
            int extra = (c >= 0xF0) ? 3 : (c >= 0xE0) ? 2 : (c >= 0xC0) ? 1 : 0;
            out[n++] = '?';
            utf8++;
            while (extra-- > 0 && ((uint8_t)*utf8 & 0xC0) == 0x80)
                utf8++;
        }
    }

    out[n] = '\0';
    return n;
}
