/**
 * @file mda_rect.h
 * @brief Rectangle Abstraction for MDA Text-Mode Regions
 * @details Combines position (x,y) and dimension (w,h) into a 32-bit packed
 * rectangle type for use in clipping, UI layout, and screen region management
 * on MDA/Hercules 80x25 displays.
 * @author Jeremy Thornton
 */
#ifndef MDA_RECT_H
#define MDA_RECT_H

#include "mda_point.h"
#include "mda_dim.h"
#include "../CONTRACT/contract.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @union mda_rect_t
 * @brief A 2D rectangle defined by position and size.
 * @details Packed 32-bit value: x,y,w,h each in one byte (little-endian).
 * Enables fast copying and direct storage in low-level display routines.
 */
typedef union {
    uint32_t packed;          /**< Raw 32-bit access for efficiency */
    struct {
        uint8_t x, y, w, h;   /**< Position (x,y) and size (w,h) as byte fields */
    };                        /**< Anonymous struct for field access */
} mda_rect_t;

static inline void mda_rect_init(mda_rect_t* r, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    require_address(r, "NULL rectangle!");
    r->x = x;                /**< Initialize x position */
    r->y = y;                /**< Initialize y position */
    r->w = w;                /**< Initialize width */
    r->h = h;                /**< Initialize height */
}

static inline mda_rect_t mda_rect_make(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    mda_rect_t r;
    mda_rect_init(&r, x, y, w, h);
    return r;                /**< Construct and return a new rectangle */
}

static inline mda_rect_t mda_rect_make_from(mda_point_t* o, mda_dim_t* d) {
    require_address(o, "NULL origin point!");
    require_address(d, "NULL dimension!");
    return mda_rect_make(o->x, o->y, d->w, d->h);  /**< Create rect from point + dim */
}

/**
 * @brief Check if a point lies within the rectangle.
 * @param r Pointer to the rectangle.
 * @param p Pointer to the point.
 * @return true if point is inside or on edge, false otherwise.
 */
bool mda_rect_contains_point(const mda_rect_t* r, mda_point_t* p);

/**
 * @brief Check if two rectangles intersect.
 * @param a First rectangle.
 * @param b Second rectangle.
 * @return true if rectangles overlap, false otherwise.
 */
bool mda_rect_intersect(const mda_rect_t* a, const mda_rect_t* b);

/**
 * @brief Compute the intersection of two rectangles.
 * @param a First rectangle.
 * @param b Second rectangle.
 * @return The overlapping rectangle, or zero-dim if no intersection.
 */
mda_rect_t mda_rect_make_intersection(const mda_rect_t* a, const mda_rect_t* b);

/**
 * @brief Clip a rectangle to stay within bounds.
 * @param rect Rectangle to modify in place.
 * @param bounds Clipping region.
 * @note If no overlap, result may have zero width or height.
 */
void mda_rect_clip(mda_rect_t* rect, const mda_rect_t* bounds);

#endif /* MDA_RECT_H */
