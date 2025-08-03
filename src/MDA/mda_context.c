#include "mda_context.h"
#include "mda_attributes.h"
#include "../CONTRACT/contract.h"
#include "../BIOS/bios_video_services.h"
#include "mda_types.h"

void mda_set_bounds(mda_context_t* ctx, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    require_address(ctx, "NULL context!");
    ctx->bounds = mda_make_rect(x, y, w, h);
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

void mda_plot(mda_rect_t* rect, uint16_t x, uint16_t y, uint16_t packed) {
    require_address(rect, "NULL rectangle!");
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax
        mov di, y
        // 2. DI = y * 80
        mov  ax, di
        shl  ax, 1       ; y * 4
        shl  ax, 1
        add  ax, di      ; y * 5
        shl  ax, 1       ; y * 5 * 16
        shl  ax, 1
        shl  ax, 1
        shl  ax, 1
        add  ax, x       ; ax = y*80 + x
        shl  ax, 1       ; word offset
        mov  di, ax
        mov  ax, packed
        cld
        stosw
    }
}

void mda_dump(FILE* stream, mda_context_t* ctx) {
    // Enforce preconditions using your DbC system
    require_address(stream, "NULL stream in mda_dump");
    require_address(ctx, "NULL context in mda_dump");

    fprintf(stream, "=== MDA Context Dump ===\n");
    fprintf(stream, "Attributes: 0x%02X\n", (unsigned)ctx->attributes);

    // Unpack mda_rect_t via union
    fprintf(stream, "Bounds: [%u,%u,%u,%u] (x=%u,y=%u,w=%u,h=%u)\n",
        (unsigned)ctx->bounds.rect.x,
        (unsigned)ctx->bounds.rect.y,
        (unsigned)ctx->bounds.rect.w,
        (unsigned)ctx->bounds.rect.h,
        (unsigned)ctx->bounds.rect.x,
        (unsigned)ctx->bounds.rect.y,
        (unsigned)ctx->bounds.rect.w,
        (unsigned)ctx->bounds.rect.h);

    fprintf(stream, "Tab Sizes: htab=%u, vtab=%u\n",
        (unsigned)ctx->htab_size, (unsigned)ctx->vtab_size);

    fprintf(stream, "BIOS Video State:\n");
    fprintf(stream, "  Columns: %u\n", (unsigned)ctx->video.columns);
    fprintf(stream, "  Mode: %u\n", (unsigned)ctx->video.mode);
    fprintf(stream, "  Active Page: %u\n", (unsigned)ctx->video.page);

    fprintf(stream, "BIOS Cursor State:\n");
    fprintf(stream, "  Cursor Scan Lines: %u-%u\n",
        (unsigned)ctx->cursor.start_scan,
        (unsigned)ctx->cursor.end_scan);
    fprintf(stream, "  Position: (%u,%u)\n",
        (unsigned)ctx->cursor.column,
        (unsigned)ctx->cursor.row);
    fprintf(stream, "  Shape: ");
    if (ctx->cursor.start_scan == 0 && ctx->cursor.end_scan == 0) {
        fprintf(stream, "hidden\n");
    } else if (ctx->cursor.start_scan == ctx->cursor.end_scan) {
        fprintf(stream, "underline (line %u)\n", (unsigned)ctx->cursor.end_scan);
    } else {
        fprintf(stream, "block (%u-%u)\n", (unsigned)ctx->cursor.start_scan, (unsigned)ctx->cursor.end_scan);
    }

    fprintf(stream, "=== End MDA Context ===\n");
}
