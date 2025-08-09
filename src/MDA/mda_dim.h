/**
 * @file mda_dim.h
 * @brief 2D Dimension Abstraction for MDA Layouts
 * @details Lightweight union type for (width, height) dimensions
 * used in text-mode regions, windows, or buffers on MDA/Hercules displays.
 * Packed layout enables efficient storage and parameter passing.
 * @author Jeremy Thornton
 */
#ifndef MDA_DIM_H
#define MDA_DIM_H

#include "../CONTRACT/contract.h"
#include <stdint.h>

/**
 * @union mda_dim_t
 * @brief A 2D dimension (width and height).
 * @details Packed 16-bit value: low byte = w, high byte = h.
 * Ideal for region sizing within a text grid.
 */
typedef union {
    uint16_t packed;          /**< Raw 16-bit access for efficiency */
    struct {
        uint8_t w;            /**< Width in columns (typically ≤ 80) */
        uint8_t h;            /**< Height in rows (typically ≤ 25) */
    };                        /**< Anonymous struct for field access */
} mda_dim_t;

static inline void mda_dim_init(mda_dim_t* d, uint8_t w, uint8_t h) {
    require_address(d, "NULL dimension!");
    d->w = w;                /**< Initialize width */
    d->h = h;                /**< Initialize height */
}

static inline mda_dim_t mda_dim_make(uint8_t w, uint8_t h) {
    mda_dim_t d;
    mda_dim_init(&d, w, h);
    return d;                /**< Construct and return a new dimension */
}

#endif /* MDA_DIM_H */
