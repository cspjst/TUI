#ifndef MDA_CONTEXT_H
#define MDA_CONTEXT_H

#include <stdbool.h>

void mda_plot(mda_rect_t* rect, uint8_t x, uint8_t y, char c, attribute_t attr);

void mda_vline(mda_rect_t* rect, uint8_t x0, uint8_t y0, , uint8_t x1, uint8_t y1, char c, attribute_t attr);

void mda_hline(mda_rect_t* rect, uint8_t x0, uint8_t y0, , uint8_t x1, uint8_t y1, char c, attribute_t attr);

void mda_box(mda_rect_t* rect, mda_rect_t* box, , char c, attribute_t attr);

void mda_box_fill(mda_rect_t* rect, mda_rect_t* box, , char c, attribute_t attr);

void mda_blit(mda_rect_t* rect_dst, mda_rect* rect_src);

bool mda_contains(mda_rect_t* rect, uint8_t x, uint8_t y);

bool mda_intersect((mda_rect_t* rect_a, mda_rect* rect_b);

#endif
