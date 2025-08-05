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
        mov bl, ds:[si]     ; BL = x
        sub bh, bh          ; BX = x
         mov al, ds:[si+1]  ; AL = y
        sub ah, ah          ; AX = y
        mov di, ax          ; DI copy y
        // 2. DI = y * 80
        shl  ax, 1           ; y * 4
        shl  ax, 1
        add  ax, di          ; y * 5
        shl  ax, 1           ; y * 5 * 16
        shl  ax, 1
        shl  ax, 1
        shl  ax, 1
        add  ax, bx          ; ax = y*80 + x
        shl  ax, 1           ; word offset
        mov  di, ax          ; ES:DI *VRAM (x,y)
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
        mov bl, ds:[si]     ; BL = p0.x
        sub bh, bh          ; BX = p0.x
        mov al, ds:[si+1]   ; AL = p0.y
        sub ah, ah          ; AX = p0.y
        mov di, ax          ; DI copy p0.y
        lds si, p1          ; DS:SI *p1
        mov cl, ds:[si]     ; CL = p1.x
        sub cl, bl          ; CL = p1.x - p0.x
        inc cl              ; CL = distance x0..x1 + 1
        sub ch, ch          ; CX = width
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
        lds  si, cell    ; DS:SI *cell
        lodsw            ; AX = cell
        cld              ; increment
        // 3. horizontal plots
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
        mov bl, ds:[si]     ; BL = p0.x
        sub bh, bh          ; BX = p0.x
        mov al, ds:[si+1]   ; AL = p0.y
        sub ah, ah          ; AX = p0.y
        mov di, ax          ; DI copy p0.y
        lds si, p1          ; DS:SI *p1
        mov cl, ds:[si+1]   ; CL = p1.y
        sub cl, al          ; CL = p1.y - p0.y
        inc cl              ; CL = distance y0..y1 + 1
        sub ch, ch          ; CX = height
        mov dx, MDA_ROW_BYTES ; DX = 160
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
        lds  si, cell    ; DS:SI *cell
        lodsw            ; AX = cell
        // 3. vertical plots
NEXT:   mov es:[di], ax
        add di, dx       ; add reg, reg is faster than add reg, imm
        loop NEXT
    }
}

void mda_draw_rect(mda_rect_t* rect, mda_cell_t* cell) {
    uint8_t a,b,c,d;
    a = b = c = d = 0;
    __asm {
        .8086
        // 1. register setup
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, rect        ; DS:SI *rect
//load up x,y,w,h into a,b,c,d and printout

        // 4. draw top line
/*
        mov dx, di           ; DX copy top left corner VRAM
        mov bx, cx           ; BX copy of width
        rep stosw            ; top line
        5. draw lhs rhs vertical lines
        mov di, dx           ; restore top left corner
        add di, MDA_ROW_BYTES    ; next line
        mov cx, (height)     ; CX = height
NEXT:   mov es:[di], ax      ; lhs cell
        mov es:[di + bx], ax ; rhs cell
        add di, MDA_ROW_BYTES    ; next line
        loop NEXT
        6. draw bottom line
        mov cx, bx           ; CX = width
        rep stosw            ; bottom line
*/
    }
     printf("(x=%i, y=%i, w=%i, h=i%\n)",a ,b ,c ,d);
}
