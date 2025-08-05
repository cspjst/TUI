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

void mda_plot(mda_point_t* point, mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, point       ; DS:SI *point
        lodsb               ; AL = x
        sub ah, ah          ; AX = x
        mov bl, ds:[si]     ; BL = y
        sub bh, bh          ; BX = y
        mov di, bx          ; DI copy y
        // 2. DI = y * 80
        shl  di, 1           ; y * 4
        shl  di, 1
        add  di, bx          ; y * 5
        shl  di, 1           ; y * 5 * 16
        shl  di, 1
        shl  di, 1
        shl  di, 1
        add  di, ax          ; ax = y*80 + x
        shl  di, 1           ; word offset ES:DI *VRAM (x,y)
        // 3. plot char:attribute
        lds  si, cell        ; DS:SI *cell
        movsw                ; *VRAM = *cell
    }
}

void mda_hline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, p0          ; DS:SI *p0
        lodsb               ; AL = p0.x
        sub ah, ah          ; AX = p0.x
        mov bl, ds:[si]     ; BL = p0.y
        sub bh, bh          ; BX = p0.y
        mov di, bx          ; DI copy p0.y
        lds si, p1          ; DS:SI *p1
        mov cl, ds:[si]     ; CL = p1.x
        sub cl, al          ; CL = p1.x - p0.x
        inc cl              ; CL = distance x0..x1 + 1
        sub ch, ch          ; CX = width
        // 2. DI = y * 80
        shl  di, 1           ; y * 4
        shl  di, 1
        add  di, bx          ; y * 5
        shl  di, 1           ; y * 5 * 16
        shl  di, 1
        shl  di, 1
        shl  di, 1
        add  di, ax          ; ax = y*80 + x
        shl  di, 1           ; word offset ES:DI *VRAM (x,y)
        // 3. draw horizontal line
        lds  si, cell        ; DS:SI *cell
        lodsw                ; AX = char:attribute pair
        cld                  ; increment
        rep stosw
    }
}

void mda_vline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, p0          ; DS:SI *p0
        lodsb               ; AL = p0.x
        sub ah, ah          ; AX = p0.x
        mov bl, ds:[si]     ; BL = p0.y
        sub bh, bh          ; BX = p0.y
        mov di, bx          ; DI copy p0.y
        lds si, p1          ; DS:SI *p1
        mov cl, ds:[si+1]   ; CL = p1.y
        sub cl, bl          ; CL = p1.y - p0.y
        inc cl              ; CL = distance y0..y1 + 1
        sub ch, ch          ; CX = height
        mov dx, MDA_ROW_BYTES
        // 2. DI = y * 80
        shl  di, 1           ; y * 4
        shl  di, 1
        add  di, bx          ; y * 5
        shl  di, 1           ; y * 5 * 16
        shl  di, 1
        shl  di, 1
        shl  di, 1
        add  di, ax          ; ax = y*80 + x
        shl  di, 1           ; word offset ES:DI *VRAM (x,y)
        // 3. draw horizontal line
        lds  si, cell        ; DS:SI *cell
        lodsw                ; AX = char:attribute pair
        // 3. vertical plots
NEXT:   mov es:[di], ax
        add di, dx       ; add reg, reg is faster than add reg, imm
        loop NEXT
    }
}

void mda_draw_rect(mda_rect_t* rect, mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, rect        ; DS:SI *rect
        lodsb               ; AL = rect.x
        sub ah, ah          ; AX = rect.x
        mov bl, ds:[si]     ; BL = rect.y
        sub bh, bh          ; BX = rect.y
        mov di, bx          ; DI copy rect.y
        mov cl, ds:[si+1]   ; CL = rect.w
        sub ch, ch          ; CX = width
        mov dl, ds:[si+2]   ; DL = rect.h
        sub dh, dh          ; DH = height
        // 2. DI = y * 80
        shl  di, 1          ; y * 4
        shl  di, 1
        add  di, bx         ; y * 5
        shl  di, 1          ; y * 5 * 16
        shl  di, 1
        shl  di, 1
        shl  di, 1
        add  di, ax         ; ax = y*80 + x
        shl  di, 1          ; word offset ES:DI *VRAM (x,y)
        // 3. preserve origin and width
        lds si, cell        ; DS:SI *cell
        lodsw               ; AX = char:attribute pair
        mov si, di          ; SI copy top left corner VRAM
        // 4. build BX into rhs vertical offset
        push cx             ; copy of width
        mov bx, cx          ; BX = width
        dec bx              ; BX = width-1
        shl bx, 1           ; BX = (width-1)*2
        // 5. draw horizontal line
        cld                 ; increment
        rep stosw
        // 5. draw lhs and rhs vertical lines
        mov di, si          ; restore top left corner
        mov cx, dx          ; CX is height loop counter
        mov dx, MDA_ROW_BYTES
        add di, dx          ; next line
NEXT:   mov es:[di], ax     ; lhs cell
        mov es:[di+bx], ax  ; rhs cell
        add di, dx          ; next line
        loop NEXT
        // 6. draw bottom line
        pop cx              ; CX = width
        rep stosw           ; bottom line
/*


NEXT:   mov es:[di], ax      ; lhs cell
        mov es:[di + bx], ax ; rhs cell
        add di, MDA_ROW_BYTES    ; next line
        loop NEXT
        6. draw bottom line
        mov cx, bx           ; CX = width
        rep stosw            ; bottom line
*/
    }
}
