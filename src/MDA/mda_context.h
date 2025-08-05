/**
 * @file mda_context.h
 * @brief MDA (Monochrome Display Adapter) context management API
 * @version 0.1.2
 * @license MIT
 * @author Jeremy Thornton
 *
 * @details
 * This module provides the core context structure and initialization routines
 * for MDA text-mode operations on IBM PC/XT/AT and compatible systems.
 */
#ifndef MDA_CONTEXT_H
#define MDA_CONTEXT_H

#include "mda_constants.h"
#include "mda_types.h"
#include "../BIOS/bios_video_services.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    mda_rect_t bounds;
    char attributes;
    uint8_t htab_size;
    uint8_t vtab_size;
    bios_video_state_t video;
    bios_cursor_state_t cursor;
    // TODO clip function here
    // TODO: mouse_state mouse; has mouse support etc
} mda_context_t;

// unbounded low level functions user must clip prior to calling

void mda_plot(mda_point_t* point, mda_cell_t* cell);

void mda_hline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell);

void mda_vline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell);

void mda_draw_rect(mda_rect_t* rect, mda_cell_t* cell);

void mda_fill_rect(mda_rect_t* rect, mda_cell_t* cell);

void mda_blit(mda_rect_t* to, mda_rect_t* from);

// context bounded functions

void mda_initialize_default_context(mda_context_t* ctx);

void mda_set_bounds(mda_context_t* ctx, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void mda_cursor_to(mda_context_t* ctx, mda_point_t* p);

void mda_cursor_advance(mda_context_t* ctx);

void mda_ascii_BEL(mda_context_t* ctx);

void mda_ascii_BS(mda_context_t* ctx);

void mda_ascii_HT(mda_context_t* ctx);

void mda_ascii_LF(mda_context_t* ctx);

void mda_ascii_VT(mda_context_t* ctx);

void mda_ascii_FF(mda_context_t* ctx);

void mda_ascii_CR(mda_context_t* ctx);

void mda_ascii_ESC(mda_context_t* ctx);

void mda_ascii_DEL(mda_context_t* ctx);

void mda_write_CRLF(mda_context_t* ctx);

void mda_write_char(mda_context_t* ctx, char chr);

void mda_scroll_up(mda_context_t* ctx);

void mda_scroll_down(mda_context_t* ctx);

void mda_scroll_left(mda_context_t* ctx);

void mda_scroll_right(mda_context_t* ctx);

#endif
