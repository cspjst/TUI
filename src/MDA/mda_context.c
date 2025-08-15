#include "mda_context.h"
#include "mda_attributes.h"
#include "mda_cell.h"
#include "mda_primitives.h"
#include "mda_rect.h"
#include "mda_control_codes.h"
#include "../CONTRACT/contract.h"
#include "../BIOS/bios_video_services.h"

void mda_set_bounds(mda_context_t* ctx, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    require_address(ctx, "NULL context!");
    ctx->bounds = mda_rect_make(x, y, w, h);
}

void mda_initialize_default_context(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    bios_set_video_mode(MDA_TEXT_MONOCHROME_80X25);
    bios_get_video_state(&ctx->video);
    bios_get_cursor_position_and_size(&ctx->cursor, ctx->video.page);
    mda_set_bounds(ctx, 0, 0, ctx->video.columns, MDA_ROWS);
    ctx->attributes = MDA_NORMAL;
    ctx->blank = mda_cell_make(' ', MDA_NORMAL);
    ctx->htab_size = MDA_DEFAULT_HTAB;
    ctx->vtab_size = MDA_DEFAULT_VTAB;
}

void mda_cursor_to(mda_context_t* ctx, mda_point_t* p) {
    require_address(ctx, "NULL context!");
    require(mda_rect_contains_point(&ctx->bounds, p), "POINT out of bounds!");
    bios_set_cursor_position(p->x, p->y, ctx->video.page);
    bios_get_cursor_position_and_size(&ctx->cursor, ctx->video.page);
}

void mda_cursor_up(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    if (ctx->cursor.row == ctx->bounds.y) {
        mda_scroll_down(&ctx->bounds, &ctx->blank);
        return;
    }
    ctx->cursor.row--;
    bios_set_cursor_position(ctx->cursor.column, ctx->cursor.row, ctx->video.page);
}

void mda_cursor_down(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    if (ctx->cursor.row == ctx->bounds.y + ctx->bounds.h - 1) {
        mda_scroll_up(&ctx->bounds, &ctx->blank);
        return;
    }
    ctx->cursor.row++;
    bios_set_cursor_position(ctx->cursor.column, ctx->cursor.row, ctx->video.page);
}

void mda_cursor_forward(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    ctx->cursor.column++;
    if (ctx->cursor.column >= ctx->bounds.x + ctx->bounds.w) { // over shoot so carriage return
        mda_CRLF(ctx);
        return;
    }
    bios_set_cursor_position(ctx->cursor.column, ctx->cursor.row, ctx->video.page);
}

void mda_cursor_back(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    if (ctx->cursor.column > ctx->bounds.x) { // Move left within current line
        ctx->cursor.column--;
    }
    else if (ctx->cursor.row > ctx->bounds.y) { // At left edge, but not at top: wrap to end of previous line
        ctx->cursor.row--;
        ctx->cursor.column = ctx->bounds.x + ctx->bounds.w - 1;
    }
    else { // At top-left corner: cannot go back — ring bell!
        mda_BEL(ctx);
    }
    bios_set_cursor_position(ctx->cursor.column, ctx->cursor.row, ctx->video.page);
}

void mda_BEL(const mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    bios_write_text_teletype_mode(ASCII_BEL, 0, ctx->video.page);
}

void mda_BS(const mda_context_t* ctx) {
    mda_cursor_back(ctx);
}

void mda_HT(const mda_context_t* ctx) {
    for(int i = 0; i < ctx->htab_size; ++i) {
        mda_cursor_forward(ctx);
    }
}

void mda_LF(const mda_context_t* ctx) {
    mda_cursor_down(ctx);
}

void mda_VT(const mda_context_t* ctx) {
    for(int i = 0; i < ctx->vtab_size; ++i){
        mda_LF(ctx);
    }
}

void mda_FF(const mda_context_t* ctx) {
    for(int i = 0; i < ctx->bounds.h; ++i){
        mda_scroll_up(&ctx->bounds, &ctx->blank);
    }
    ctx->cursor.row = ctx->bounds.y;
    ctx->cursor.column = ctx->bounds.x;
    bios_set_cursor_position(ctx->cursor.column, ctx->cursor.row, ctx->video.page);
}

void mda_CR(const mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    ctx->cursor.column = ctx->bounds.x;
    bios_set_cursor_position(ctx->cursor.column, ctx->cursor.row, ctx->video.page);
}

void mda_ESC(const mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    // set esc char mode
}


void mda_DELconst (mda_context_t* ctx) {
    mda_BS(ctx);
    mda_print_char(ctx, ' ');
    mda_BS(ctx);
}

void mda_CRLF(const mda_context_t* ctx) {
    mda_CR(ctx);
    mda_LF(ctx);
}

void mda_print_char(const mda_context_t* ctx, char chr) {
    require_address(ctx, "NULL context!");
    bios_write_character_and_attribute_at_cursor(chr, ctx->attributes, 1, ctx->video.page);
    mda_cursor_forward(ctx);
}

void mda_print_string(const mda_context_t* ctx, char* str) {
    require_address(ctx, "NULL context!");
    require_address(str, "NULL string!");

    char c;
    while ((c = *str++) != '\0') {
        if (c == '\\') {
            c = *str++;
            switch (c) {
                case 'a':  // Alert/Bell
                    mda_BEL(ctx);
                    break;
                case 'b':  // Backspace
                    mda_BS(ctx);
                    break;
                case 't':  // Horizontal Tab
                    mda_HT(ctx);
                    break;
                case 'n':  // Newline (LF)
                    mda_LF(ctx);
                    break;
                case 'v':  // Vertical Tab
                    mda_VT(ctx);
                    break;
                case 'f':  // Form Feed
                    mda_FF(ctx);
                    break;
                case 'r':  // Carriage Return
                    mda_CR(ctx);
                    break;
                case '\\': // Backslash
                    mda_print_char(ctx, '\\');
                    break;
                default:  // Unknown escape
                    break;
            }
        }
        else {
            mda_print_char(ctx, c);
        }
    }
}
