#ifndef MDA_CONTEXT_H
#define MDA_CONTEXT_H

#include mda_constants.h
#include mda_types.h
#include <stdbool.h>

typedef struct {
    char attributes;
    mda_rect_t bounds;
    uint8_t htab_size;
    uint8_t vtab_size;
    bios_video_state_t video;
    bios_cursor_state_t cursor;
    // TODO: mouse_state mouse; has mouse support etc
} mda_context_t;

void mda_initialize_default_context(mda_context_t* ctx);

void mda_set_bounds(mda_context_t* ctx, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void mda_plot(mda_rect_t* rect, uint8_t x, uint8_t y,  mda_char_attr_t chatt);

void mda_vline(mda_rect_t* rect, uint8_t x0, uint8_t y0, , uint8_t x1, uint8_t y1,  mda_char_attr_t chatt);

void mda_hline(mda_rect_t* rect, uint8_t x0, uint8_t y0, , uint8_t x1, uint8_t y1,  mda_char_attr_t chatt);

void mda_box(mda_rect_t* rect, mda_rect_t* box, , mda_char_attr_t chatt);

void mda_box_fill(mda_rect_t* rect, mda_rect_t* box, , mda_char_attr_t chatt);

void mda_blit(mda_rect_t* rect_dst, mda_rect* rect_src);

bool mda_contains(mda_rect_t* rect, uint8_t x, uint8_t y);

bool mda_intersect(mda_rect_t* rect_a, mda_rect* rect_b);

void mda_cursor_to(mda_context_t* ctx, uint8_t x, uint8_t y);

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

#endif
