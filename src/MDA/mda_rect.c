#include "mda_rect.h"

mda_rect_t mda_make_rect_from_points(mda_point_t top_left, mda_point_t bottom_right) {
    require_range(top_left.pos.x < bottom_right.pos.x && top_left.pos.y < bottom_right.pos.y, "CORNERS out of range!");
    mda_rect_t r;
    mda_rect_init(&r, top_left.pos.x, top_left.pos.y, bottom_right.pos.x - top_left.pos.x, bottom_right.pos.y - top_left.pos.y);
    return r;
}

bool mda_rect_contains_point(const mda_rect_t* r, mda_point_t p) {
    require_address(r, "NULL rectangle!");
    return p.pos.x >= r->rect.x &&
           p.pos.y >= r->rect.y &&
           p.pos.x <  r->rect.x + r->rect.w &&
           p.pos.y <  r->rect.y + r->rect.h;
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

    if (left >= right || top >= bottom)
        return mda_make_rect(0, 0, 0, 0);

    return mda_make_rect(left, top, right - left, bottom - top);
}
