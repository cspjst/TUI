#include "mda_context.h"
#include "mda_attributes.h"
#include "mda_rect.h"
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
    ctx->htab_size = MDA_DEFAULT_HTAB;
    ctx->vtab_size = MDA_DEFAULT_VTAB;
}

void mda_cursor_to(mda_context_t* ctx, mda_point_t* p) {
    require_address(ctx, "NULL context!");
    require(mda_rect_contains_point(&ctx->bounds, p), "POINT out of bounds!");
    bios_set_cursor_position(x, y, ctx->video.page);
    bios_get_cursor_position_and_size(&ctx->cursor, ctx->video.page);
}

void mda_cursor_up(mda_context_t* ctx) {

}

void mda_cursor_down(mda_context_t* ctx) {

}

void mda_cursor_forward(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    ctx->cursor.column++;
    if(ctx->cursor.column == (ctx->bounds.x + ctx->bounds.width)) {
        mda_print_CRLF(ctx);
        return;
    }
    bios_set_cursor_position(x, y, ctx->video.page);
    bios_get_cursor_position_and_size(&ctx->cursor, ctx->video.page);
}

void mda_cursor_back(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    if(ctx->cursor.column == ctx->bounds.x) {
        ctx->cursor.column = ctx->bounds.x + ctx->bounds.width 
        if(ctx->cursor.row = ctx->bounds.y) {
            // scroll down
        }
    }
    bios_set_cursor_position(x, y, ctx->video.page);
    bios_get_cursor_position_and_size(&ctx->cursor, ctx->video.page);
}

void mda_ctrl_BEL(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    bios_write_text_teletype_mode(ASCII_BEL, 0, ctx->video.page);
}

void mda_ctrl_BS(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    if(ctx->cursor.column > ctx->x
     if(ctx->cursor.column > ctx->x) {
        ctx->cursor.column--;
        bios_set_cursor_position(ctx->cursor.column, ctx->cursor.row, ctx->video.page);
    }
}

//void mda_ctrl_HT(mda_context_t* ctx);   ///< Horizontal tab: advance to next HT stop

void mda_ctrl_LF(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    mda_cursor_down(ctx);
    bios_set_cursor_position(x, y, ctx->video.page);
    bios_get_cursor_position_and_size(&ctx->cursor, ctx->video.page);
}

//void mda_ctrl_VT(mda_context_t* ctx);   ///< Vertical Tab: advance down by vtab_size

//void mda_ctrl_FF(mda_context_t* ctx);   ///< Form Feed: clear screen, home cursor

void mda_ctrl_CR(mda_context_t* ctx) {
    require_address(ctx, "NULL context!");
    ctx->cursor.column = ctx->x;
    bios_set_cursor_position(x, y, ctx->video.page);
    bios_get_cursor_position_and_size(&ctx->cursor, ctx->video.page);
} 

//void mda_ctrl_ESC(mda_context_t* ctx);  ///< Escape: begin control sequence (stub)

//void mda_ctrl_DEL(mda_context_t* ctx);

//void mda_print_char(mda_context_t* ctx, char chr);  ///< Write char with current attr

void mda_print_CRLF(mda_context_t* ctx) {
    mda_ctrl_CR(ctx);
    mda_ctrl_LF(ctx);
}
