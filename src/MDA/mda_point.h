#ifndef MDA_POINT_H
#define MDA_POINT_H

#include "../CONTRACT/contract.h"
#include <stdio.h>
#include <stdint.h>

typedef union {
    uint16_t packed;
    struct {
        uint8_t y, x;
    } pos;
} mda_point_t;

static inline void mda_point_init(mda_point_t* p, uint8_t x, uint8_t y) {
    require_address(p, "NULL point!");
    p->pos.x = x;
    p->pos.y = y;
}

static inline mda_point_t mda_point_make(uint8_t x, uint8_t y) {
    mda_point_t p;
    mda_point_init(&p, x, y);
    return p;
}

static inline mda_point_t mda_point_add(mda_point_t a, mda_point_t b) {
    return mda_point_make(a.pos.x + b.pos.x, a.pos.y + b.pos.y);
}

static inline void print_point(mda_point_t p) {
    printf("(%i, %i)", p.pos.x, p.pos.y);
}

#endif
