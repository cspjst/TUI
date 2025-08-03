#include "mda_context.h"
#include "mda_attributes.h"
#include "../CONTRACT/contract.h"
#include "../BIOS/bios_video_services.h"
//#include "mda_types.h"

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

void mda_plot(uint16_t point, uint16_t cell) {
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax
        mov ax, point
        mov bl, ah      ; BL = x
        sub bh, bh      ; BX = x
        sub ah, ah      ; AX = y
        mov di, ax
        // 2. DI = y * 80
        shl  ax, 1       ; y * 4
        shl  ax, 1
        add  ax, di      ; y * 5
        shl  ax, 1       ; y * 5 * 16
        shl  ax, 1
        shl  ax, 1
        shl  ax, 1
        add  ax, bx      ; ax = y*80 + x
        shl  ax, 1       ; word offset
        mov  di, ax
        mov  ax, cell
        cld
        stosw
    }
}

void mda_hline(uint16_t p0, uint16_t p1,uint16_t cell) {
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax
        mov ax, p0
        mov bl, ah      ; BL = p0.x
        sub bh, bh      ; BX = p0.x
        sub ah, ah      ; AX = p0.y
        mov di, ax
        mov cx, p1
        xchg cl, ch     ; CL = p1.x
        sub ch, ch      ; CX = p1.x
        cmp cl, bl
        jg OK
        xchg cl, bl
OK:     sub cl, bl
        inc cl
        // 2. DI = y * 80
        shl  ax, 1       ; y * 4
        shl  ax, 1
        add  ax, di      ; y * 5
        shl  ax, 1       ; y * 5 * 16
        shl  ax, 1
        shl  ax, 1
        shl  ax, 1
        add  ax, bx      ; ax = y*80 + x
        shl  ax, 1       ; word offset
        mov  di, ax
        mov  ax, cell
        cld
        rep stosw
    }
}

void mda_vline(uint16_t p0, uint16_t p1,uint16_t cell) {
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax
        mov ax, p0
        mov bl, ah      ; BL = p0.x
        sub bh, bh      ; BX = p0.x
        sub ah, ah      ; AX = p0.y
        mov di, ax
        mov dx, MDA_ROW_BYTES   ; 160
        mov cx, p1
        sub ch, ch      ; CX = p1.y
        cmp cl, al
        jg OK
        xchg cl, al
OK:     sub cl, al
        inc cl
        // 2. DI = y * 80
        shl  ax, 1       ; y * 4
        shl  ax, 1
        add  ax, di      ; y * 5
        shl  ax, 1       ; y * 5 * 16
        shl  ax, 1
        shl  ax, 1
        shl  ax, 1
        add  ax, bx      ; ax = y*80 + x
        shl  ax, 1       ; word offset
        mov  di, ax
        mov  ax, cell
        cld
        // 3. vertical plots
NEXT:   mov es:[di], ax
        add di, dx
        loop NEXT
    }
}

void mda_draw_rect(uint16_t top_left, uint16_t bottom_right, uint16_t cell) {

}
