#ifndef MDA_DIM_H
#define MDA_DIM_H

#include "../CONTRACT/contract.h"
#include <stdint.h>

typedef union {
    uint16_t packed;
    struct {
        uint8_t w, h;
    };
} mda_dim_t;

static inline void mda_dim_init(mda_dim_t* d, uint8_t w, uint8_t h) {
    require_address(d, "NULL dimension!");
    d->w = w;
    d->h = h;
}

static inline mda_dim_t mda_dim_make(uint8_t w, uint8_t h) {
    mda_dim_t d;
    mda_dim_init(&d, w, h);
    return d;
}

#endif
