#ifndef MDA_RECT_H
#define MDA_RECT_H

#include "../CONTRACT/contract.h"
#include <stdint.h>
#include <stdbool.h>

typedef union {
    uint32_t packed;
    struct {
        uint8_t x;
        uint8_t y;
        uint8_t w;
        uint8_t h;
    } rect;
} mda_rect_t;

static inline void mda_rect_init(mda_rect_t* r, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    require_address(r, "NULL rectangle!");
    r->rect.x = x;
    r->rect.y = y;
    r->rect.w = w;
    r->rect.h = h;
}

static inline mda_rect_t mda_make_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    mda_rect_t r;
    mda_rect_init(&r, x, y, w, h);
    return r;
}

bool mda_rect_contains(const mda_rect_t* r, uint8_t x, uint8_t y);

bool mda_rect_intersect(const mda_rect_t* a, const mda_rect_t* b);

mda_rect_t mda_rect_intersection(const mda_rect_t* a, const mda_rect_t* b);

#endif
