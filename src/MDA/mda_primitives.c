#include "mda_primitives.h"
#include "mda_constants.h"

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

void mda_draw_hline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell) {
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
        shl  di, 1          ; y * 4
        shl  di, 1
        add  di, bx         ; y * 5
        shl  di, 1          ; y * 5 * 16
        shl  di, 1
        shl  di, 1
        shl  di, 1
        add  di, ax         ; ax = y*80 + x
        shl  di, 1          ; word offset ES:DI *VRAM (x,y)
        // 3. setup cell
        lds  si, cell       ; DS:SI *cell
        lodsw               ; AX = char:attribute pair
        // 4. draw horizontal line
        cld                 ; increment REP
        rep stosw           ; draw hline
    }
}

void mda_draw_vline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell) {
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
        shl  di, 1          ; y * 4
        shl  di, 1
        add  di, bx         ; y * 5
        shl  di, 1          ; y * 5 * 16
        shl  di, 1
        shl  di, 1
        shl  di, 1
        add  di, ax         ; ax = y*80 + x
        shl  di, 1          ; word offset ES:DI *VRAM (x,y)
        // 3. setup cell
        lds  si, cell       ; DS:SI *cell
        lodsw               ; AX = char:attribute pair
        // 4. vertical plots
NEXT:   mov es:[di], ax
        add di, dx          ; add reg, reg is faster than add reg, imm
        loop NEXT
    }
}

void mda_draw_hline_caps(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cells) {
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
        shl  di, 1          ; y * 4
        shl  di, 1
        add  di, bx         ; y * 5
        shl  di, 1          ; y * 5 * 16
        shl  di, 1
        shl  di, 1
        shl  di, 1
        add  di, ax         ; ax = y*80 + x
        shl  di, 1          ; word offset ES:DI *VRAM (x,y)
        // 3. setup cell
        lds  si, cells      ; DS:SI *cells list of chars lhs,line,rhs
        dec  cx
        jcxz ONE
        dec  cx
        // 4. draw horizontal line
        cld                 ; increment
        movsw               ; *ES:DI++ = *DS:SI++ (LHS end cap char)
        lodsw               ; AX = *DS:SI++ (line char)
        rep stosw           ; draw hline  *ES:DI++ = AX
ONE:    movsw               ; *ES:DI++ = *DS:SI++ (RHS end cap char)
    }
}

void mda_draw_vline_caps(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cells) {
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
        shl  di, 1          ; y * 4
        shl  di, 1
        add  di, bx         ; y * 5
        shl  di, 1          ; y * 5 * 16
        shl  di, 1
        shl  di, 1
        shl  di, 1
        add  di, ax         ; ax = y*80 + x
        shl  di, 1          ; word offset ES:DI *VRAM (x,y)
        // 3. setup cell
        lds  si, cells      ; DS:SI *cells
        dec  cx
        jcxz ONE
        dec  cx
        // 4. vertical plots
        lodsw               ; AX = *DS:SI++
        mov es:[di], ax     ; AX = top end cap
        add di, dx
        lodsw               ; AX = *DS:SI++
        jcxz ONE
NEXT:   mov es:[di], ax     ; AX = vertical char
        add di, dx          ; add reg, reg is faster than add reg, imm
        loop NEXT
ONE:    lodsw               ; AX = *DS:SI++
        mov es:[di], ax     ; AX = bottom end cap

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
        sub dh, dh          ; DX = height
        sub dx, 2           ; height-2
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
        // 3. setup cell
        lds si, cell        ; DS:SI *cell
        lodsw               ; AX = char:attribute pair
        // 4. set rep counters,
        mov si, di          ; SI copy of *VRAM
        mov bx, cx          ; BX copy of width
        cld                 ; increment REP
        // 5. draw top horizontal line
        rep stosw
        mov cx, bx          ; restore CX width counter
        mov di, si          ; restore *VRAM top left corner
        // 6. setup registers for vertical lines
        dec bx              ; BX = width-1
        shl bx, 1           ; BX = (width-1)*2
        mov si, MDA_ROW_BYTES   ; SI = 160
        add di, si          ; next line
        // 7. draw lhs and rhs vertical lines between hlines
NEXT:   mov es:[di], ax     ; lhs cell
        mov es:[di+bx], ax  ; rhs cell
        add di, si          ; next line *VRAM + 160
        dec dx
        jnz NEXT
        // 8. draw bottom horizontal line
        rep stosw           ; bottom line
    }
}

void mda_fill_rect(mda_rect_t* rect, mda_cell_t* cell) {
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
        sub dh, dh          ; DX = height
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
        lds si, cell        ; DS:SI *cell
        lodsw               ; AX = char:attribute pair
        // 3. setup cell
        lds  si, cell       ; DS:SI *cell
        lodsw               ; AX = char:attribute pair
        // 4. calculate next line offset
        mov si, MDA_ROW_BYTES   ; SI = 160
        sub si, cx          ; SI = 160 - (width * 2)
        sub si, cx
        // 5. set rep counters,
        mov bx, cx          ; BX copy of width
        cld                 ; increment REP
        // 6. draw horizontal lines length CX height times
 NEXT:  mov cx, bx          ; restore width
        rep stosw           ; draw hline
        add di, si   ; next line *VRAM + 160 - width
        dec dx
        jnz NEXT
    }
}
