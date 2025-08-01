#include mda_context.h
#include "../CONTRACT/contract.h"
#include "../BIOS/bios_video_services.h"

void mda_set_bounds(mda_context_t* ctx, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  ctx->bounds.rect.x = x;
  ctx->bounds.rect.y = y;
  ctx->bounds.rect.w = w;
  ctx->bounds.rect.h = h;
}

void mda_initialize_default_context(mda_context_t* ctx) {
    require_addr(ctx, "NULL context!");
    bios_set_video_mode(MDA_TEXT_MONOCHROME_80X25);
    bios_get_video_state(&ctx->video);
    bios_get_cursor_position_and_size(&ctx->cursor, ctx->video.page);
    mda_set_bounds(ctx, 0, 0, ctx->video.columns, MDA_SCREEN_ROWS);
    ctx->attributes = MDA_NORMAL;
    ctx->htab_size = MDA_DEFAULT_HTAB;
    ctx->vtab_size = MDA_DEFAULT_VTAB;
}

void mda_plot(mda_rect_t* rect, uint16_t x, uint16_t y, mda_cell_t cell) {
    require_addr(ctx, "NULL context!");
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax
        mov di, y
        // 2. DI = y * 80
        mov  ax, di      
        shl  ax, 2       ; y * 4
        add  ax, di      ; y * 5
        shl  ax, 4       ; y * 5 * 16
        add  ax, x       ; ax = y*80 + x
        shl  ax, 1       ; word offset
        mov  di, ax    
        mov  ax, cell 
        cld
        stosw
        
    }
}
