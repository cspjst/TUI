#ifndef MDA_RECT_H
#define MDA_RECT_H

#include "mda_point.h"
#include "mda_dim.h"
#include "../CONTRACT/contract.h"
#include <stdint.h>
#include <stdbool.h>

typedef union {
    uint32_t packed;
    struct {
        mda_dim_t dim;
        mda_point_t pos;
    } parts;
    struct {
        uint8_t y, x, h, w;
    } rect;
} mda_rect_t;

static inline void mda_rect_init(mda_rect_t* r, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    require_address(r, "NULL rectangle!");
    r->rect.x = x;
    r->rect.y = y;
    r->rect.w = w;
    r->rect.h = h;
}

static inline mda_rect_t mda_rect_make(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    mda_rect_t r;
    mda_rect_init(&r, x, y, w, h);
    return r;
}

static inline mda_rect_t mda_rect_make_from(mda_point_t* o, mda_dim_t* d) {
    return mda_rect_make(o->pos.x, o->pos.y, d->dim.w, d->dim.h);
}

bool mda_rect_contains_point(const mda_rect_t* r, mda_point_t* p);

bool mda_rect_intersect(const mda_rect_t* a, const mda_rect_t* b);

mda_rect_t mda_rect_make_intersection(const mda_rect_t* a, const mda_rect_t* b);

void mda_rect_clip(mda_rect_t* rect, const mda_rect_t* bounds);

#endif
