#include "mda_rect.h"

bool mda_rect_contains_point(const mda_rect_t* r, mda_point_t* p) {
    require_address(r, "NULL rectangle!");
    return p->x >= r->x &&
           p->y >= r->y &&
           p->x <  r->x + r->w &&
           p->y <  r->y + r->h;
}

bool mda_rect_intersect(const mda_rect_t* a, const mda_rect_t* b) {
    require_address(a, "NULL rectangle 'a'!");
    require_address(b, "NULL rectangle 'b'!");
    return !(   a->x >= b->x + b->w ||
                b->x >= a->x + a->w ||
                a->y >= b->y + b->h ||
                b->y >= a->y + a->h
            );
}

mda_rect_t mda_rect_make_intersection(const mda_rect_t* a, const mda_rect_t* b) {
    require_address(a, "NULL rectangle 'a'!");
    require_address(b, "NULL rectangle 'b'!");
    uint8_t left   = (a->x > b->x) ? a->x : b->x;
    uint8_t right  = (a->x + a->w < b->x + b->w) ? a->x + a->w : b->x + b->w;
    uint8_t top    = (a->y > b->y) ? a->y : b->y;
    uint8_t bottom = (a->y + a->h < b->y + b->h) ? a->y + a->h : b->y + b->h;
    return mda_rect_make(left, top, right - left, bottom - top); // UB if do not intersect
}
