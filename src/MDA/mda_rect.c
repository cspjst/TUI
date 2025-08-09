/**
 * @file mda_rect.c
 * @brief Implementation of MDA Rectangle Operations
 * @details Provides functions for rectangle intersection, containment,
 * and clipping — used in text-mode UI layout and screen region management
 * for MDA/Hercules displays.
 * @author Jeremy Thornton
 */
#include "mda_rect.h"

bool mda_rect_contains_point(const mda_rect_t* r, mda_point_t* p) {
    require_address(r, "NULL rectangle!");
    return p->x >= r->x &&       /**< inclusive left */
           p->y >= r->y &&       /**< inclusive top */
           p->x <  r->x + r->w &&/**< exclusive right */
           p->y <  r->y + r->h;  /**< exclusive bottom */
}

bool mda_rect_intersect(const mda_rect_t* a, const mda_rect_t* b) {
    require_address(a, "NULL rectangle 'a'!");
    require_address(b, "NULL rectangle 'b'!");
    return !(                     /**< no overlap if: */
                a->x >= b->x + b->w ||  /**< A is right of B */
                b->x >= a->x + a->w ||  /**< B is right of A */
                a->y >= b->y + b->h ||  /**< A is below B */
                b->y >= a->y + a->h     /**< B is below A */
            );
}

mda_rect_t mda_rect_make_intersection(const mda_rect_t* a, const mda_rect_t* b) {
    require_address(a, "NULL rectangle 'a'!");
    require_address(b, "NULL rectangle 'b'!");
    uint8_t left   = (a->x > b->x) ? a->x : b->x;
    uint8_t right  = (a->x + a->w < b->x + b->w) ? a->x + a->w : b->x + b->w;
    uint8_t top    = (a->y > b->y) ? a->y : b->y;
    uint8_t bottom = (a->y + a->h < b->y + b->h) ? a->y + a->h : b->y + b->h;
    ensure(right >= left,  "INVALID intersection: right < left!");
    ensure(bottom >= top, "INVALID intersection: bottom < top!");
    return mda_rect_make(left, top, right - left, bottom - top);
}

void mda_rect_clip(mda_rect_t* rect, const mda_rect_t* bounds) {
    require_address(rect, "NULL rectangle!");
    require_address(bounds, "NULL bounds!");
    *rect = mda_rect_make_intersection(rect, bounds);  /**< clip in place */
}
