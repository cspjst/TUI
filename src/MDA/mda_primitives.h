#ifndef MDA_PRIMITIVES_H
#define MDA_PRIMITIVES_H

#include "mda_types.h"

// unbounded low level functions user must clip prior to calling

void mda_plot(mda_point_t* point, mda_cell_t* cell);

void mda_draw_hline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell);

void mda_draw_vline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell);

// (3 cells) = lhs end cap, hline, rhs end cap
void mda_draw_hline_caps(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cells);

// (3 cells) - top end cap, vline, bottom end cap
void mda_draw_vline_caps(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cells);

void mda_draw_rect(mda_rect_t* rect, mda_cell_t* cell);

void mda_fill_rect(mda_rect_t* rect, mda_cell_t* cell);

// (8 cells) = top-left,top-hline,top-right,vline-left,vline-right,bottom-left,bottom-hline,bottom-right
void mda_draw_border(mda_rect_t* rect, mda_cell_t* cells);

void mda_blit(mda_rect_t* to, mda_rect_t* from);

#endif
