#ifndef MDA_POINT_H
#define MDA_POINT_H

#include "../CONTRACT/contract.h"
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

static inline mda_point_t mda_make_point(uint8_t x, uint8_t y) {
    mda_point_t p;
    mda_point_init(&p, x, y);
    return p;
}

#endif
