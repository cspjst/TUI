#include "mda_rect.h"

bool mda_rect_contains_point(const mda_rect_t* r, mda_point_t* p) {
    require_address(r, "NULL rectangle!");
    return p->pos.x >= r->rect.x &&
           p->pos.y >= r->rect.y &&
           p->pos.x <  r->rect.x + r->rect.w &&
           p->pos.y <  r->rect.y + r->rect.h;
}

bool mda_rect_intersect(const mda_rect_t* a, const mda_rect_t* b) {
    require_address(a, "NULL rectangle 'a'!");
    require_address(b, "NULL rectangle 'b'!");
    return !(   a->rect.x >= b->rect.x + b->rect.w ||
                b->rect.x >= a->rect.x + a->rect.w ||
                a->rect.y >= b->rect.y + b->rect.h ||
                b->rect.y >= a->rect.y + a->rect.h
            );
}

mda_rect_t mda_rect_make_intersection(const mda_rect_t* a, const mda_rect_t* b) {
    require_address(a, "NULL rectangle 'a'!");
    require_address(b, "NULL rectangle 'b'!");

    uint8_t left   = (a->rect.x > b->rect.x) ? a->rect.x : b->rect.x;
    uint8_t right  = (a->rect.x + a->rect.w < b->rect.x + b->rect.w) ? a->rect.x + a->rect.w : b->rect.x + b->rect.w;
    uint8_t top    = (a->rect.y > b->rect.y) ? a->rect.y : b->rect.y;
    uint8_t bottom = (a->rect.y + a->rect.h < b->rect.y + b->rect.h) ? a->rect.y + a->rect.h : b->rect.y + b->rect.h;

    if (left >= right || top >= bottom) {
        mda_rect_init(&r, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
        return r;
    }

    return mda_rect_make(left, top, right - left, bottom - top);
}
