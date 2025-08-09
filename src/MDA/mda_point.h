/**
 * @file mda_point.h
 * @brief 2D Point Abstraction for MDA Coordinate System
 * @details Lightweight union type for (x,y) coordinates used in text-mode
 * cursor and cell positioning on MDA/Hercules displays (80x25 grid).
 * Packed for efficient passing and memory layout.
 * @author Jeremy Thornton
 */
#ifndef MDA_POINT_H
#define MDA_POINT_H

#include "../CONTRACT/contract.h"
#include <stdio.h>
#include <stdint.h>

/**
 * @union mda_point_t
 * @brief A 2D point in MDA display coordinates.
 * @details Packed 16-bit value: low byte = x, high byte = y.
 * Enables fast copying and direct storage in video memory layouts.
 */
typedef union {
    uint16_t packed;          /**< Raw 16-bit access for efficiency */
    struct {
        uint8_t x;            /**< Column position (0 to 79) */
        uint8_t y;            /**< Row position (0 to 24) */
    };                        /**< Anonymous struct for field access */
} mda_point_t;

static inline void mda_point_init(mda_point_t* p, uint8_t x, uint8_t y) {
    require_address(p, "NULL point!");
    p->x = x;                /**< Initialize x coordinate */
    p->y = y;                /**< Initialize y coordinate */
}

static inline mda_point_t mda_point_make(uint8_t x, uint8_t y) {
    mda_point_t p;
    mda_point_init(&p, x, y);
    return p;                /**< Construct and return a new point */
}

static inline mda_point_t mda_point_add(mda_point_t a, mda_point_t b) {
    return mda_point_make(a.x + b.x, a.y + b.y);  /**< Add x and y components */
}

#endif /* MDA_POINT_H */
