/**
 * @file mda_context.h
 * @brief MDA (Monochrome Display Adapter) context management API
 * @version 0.1.1
 * @license MIT
 * @author Jeremy Thornton
 *
 * @details
 * This module provides the core context structure and initialization routines
 * for MDA text-mode operations on IBM PC/XT/AT and compatible systems.
 *
 * The mda_context_t encapsulates:
 * - Current text bounds and clipping region
 * - Character attributes (intensity, blink, etc.)
 * - Tab settings (horizontal and vertical)
 * - BIOS video and cursor state
 *
 * All rendering and cursor operations are performed relative to the active context.
 * Functions in this module enforce design-by-contract using require_address().
 *
 * Example usage:
 * @code
 * mda_context_t ctx;
 * mda_initialize_default_context(&ctx);
 * mda_plot(&ctx.bounds, 40, 12, mda_make_cell('X', ctx.attributes).packed);
 * @endcode
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

void mda_plot(uint16_t point, uint16_t cell);

void mda_hline(uint16_t p0, uint16_t p1,uint16_t cell);

void mda_vline(uint16_t p0, uint16_t p1,uint16_t cell);

void mda_draw_rect(mda_rect_t* box, mda_cell_t cell);

void mda_draw_rect_fill(mda_rect_t* box, mda_cell_t cell);

void mda_blit(mda_rect_t* rect_dst, mda_rect_t* rect_src);

bool mda_contains(mda_rect_t* bounds, uint16_t point);

bool mda_intersect(mda_rect_t* rect_a, mda_rect_t* rect_b);

// context bounded functions

void mda_initialize_default_context(mda_context_t* ctx);

void mda_set_bounds(mda_context_t* ctx, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void mda_cursor_to(mda_context_t* ctx, mda_point_t p);

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

void mda_dump(FILE* stream, mda_context_t* ctx);

#endif
