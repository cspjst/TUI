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
