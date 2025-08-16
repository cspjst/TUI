/**
 * @file mda_primitives.c
 * @brief 8086 Assembly Implementation of MDA Text-Mode Drawing Primitives
 * @details Low-level, unbounded rendering functions optimized for IBM PC/XT
 * (8088 @ 4.77 MHz). Uses direct video memory access at segment B000h
 * for maximum performance in monochrome text mode.
 *
 * All functions assume caller has clipped coordinates to 80x25 or context bounds.
 *
 * @note Hand-tuned for minimal instruction count and cycle usage.
 * @author Jeremy Thornton
 */
#include "mda_primitives.h"
#include "mda_cell.h"
#include "mda_constants.h"

mda_cell_t* mda_as_pointer(const mda_point_t* point) {
    mda_cell_t* pcell = 0;
    mda_cell_t** ppcell = &pcell;
    __asm {
        .8086
        // 1. register setup (no flags used)
        lds si, point       ; DS:SI *point
        lodsb               ; AL = x
        xor ah, ah          ; AX = x
        mov bl, ds:[si]     ; BL = y
        xor bh, bh          ; BX = y
        mov dx, bx          ; DX copy y
        // 2. DI = y * 80
        shl  dx, 1           ; y * 4
        shl  dx, 1
        add  dx, bx          ; y * 5
        shl  dx, 1           ; y * 5 * 16
        shl  dx, 1
        shl  dx, 1
        shl  dx, 1
        add  dx, ax          ; ax = y*80 + x
        shl  dx, 1           ; word offset ES:DI *VRAM (x,y)
        // 3. setup ptr
        les  di, ppcell
        mov  es:[di], dx
        mov  es:[di+2], MDA_SEGMENT
    }
    return pcell;
}

void mda_plot(const mda_point_t* point, const mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register setup (no flags used)
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, point       ; DS:SI *point
        lodsb               ; AL = x
        xor ah, ah          ; AX = x
        mov bl, ds:[si]     ; BL = y
        xor bh, bh          ; BX = y
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

void mda_draw_hline(const mda_point_t* p0, const mda_point_t* p1, const mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        cld                 ; inc str ops
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, p0          ; DS:SI *p0
        lodsb               ; AL = p0.x
        xor ah, ah          ; AX = p0.x
        mov bl, ds:[si]     ; BL = p0.y
        xor bh, bh          ; BX = p0.y
        mov di, bx          ; DI copy p0.y
        lds si, p1          ; DS:SI *p1
        mov cl, ds:[si]     ; CL = p1.x
        sub cl, al          ; CL = p1.x - p0.x
        inc cl              ; CL = distance x0..x1 + 1
        xor ch, ch          ; CX = width
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
        popf                ; restore flags
    }
}

void mda_draw_vline(const mda_point_t* p0, const mda_point_t* p1, const mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        cld                 ; inc str ops
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, p0          ; DS:SI *p0
        lodsb               ; AL = p0.x
        xor ah, ah          ; AX = p0.x
        mov bl, ds:[si]     ; BL = p0.y
        xor bh, bh          ; BX = p0.y
        mov di, bx          ; DI copy p0.y
        lds si, p1          ; DS:SI *p1
        mov cl, ds:[si+1]   ; CL = p1.y
        sub cl, bl          ; CL = p1.y - p0.y
        inc cl              ; CL = distance y0..y1 + 1
        xor ch, ch          ; CX = height
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
        popf                ; restore flags
    }
}

void mda_draw_hline_caps(const mda_point_t* p0, const mda_point_t* p1, const mda_cell_t* cells) {
    __asm {
        .8086
        pushf
        // 1. register & flag setup
        pushf
        cld                 ; inc str ops
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, p0          ; DS:SI *p0
        lodsb               ; AL = p0.x
        xor ah, ah          ; AX = p0.x
        mov bl, ds:[si]     ; BL = p0.y
        xor bh, bh          ; BX = p0.y
        mov di, bx          ; DI copy p0.y
        lds si, p1          ; DS:SI *p1
        mov cl, ds:[si]     ; CL = p1.x
        sub cl, al          ; CL = p1.x - p0.x
        inc cl              ; CL = distance x0..x1 + 1
        xor ch, ch          ; CX = width
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
        movsw               ; *ES:DI++ = *DS:SI++ (LHS end cap char)
        lodsw               ; AX = *DS:SI++ (line char)
        rep stosw           ; draw hline  *ES:DI++ = AX
ONE:    movsw               ; *ES:DI++ = *DS:SI++ (RHS end cap char)
        popf                ; restore flags
    }
}

void mda_draw_vline_caps(const mda_point_t* p0, const mda_point_t* p1, const mda_cell_t* cells) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        cld                 ; inc str ops
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, p0          ; DS:SI *p0
        lodsb               ; AL = p0.x
        xor ah, ah          ; AX = p0.x
        mov bl, ds:[si]     ; BL = p0.y
        xor bh, bh          ; BX = p0.y
        mov di, bx          ; DI copy p0.y
        lds si, p1          ; DS:SI *p1
        mov cl, ds:[si+1]   ; CL = p1.y
        sub cl, bl          ; CL = p1.y - p0.y
        inc cl              ; CL = distance y0..y1 + 1
        xor ch, ch          ; CX = height
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
        popf                ; restore flags
    }
}

void mda_draw_rect(const mda_rect_t* rect, const mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        cld                 ; inc str ops
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, rect        ; DS:SI *rect
        lodsb               ; AL = rect.x
        xor ah, ah          ; AX = rect.x
        mov bl, ds:[si]     ; BL = rect.y
        xor bh, bh          ; BX = rect.y
        mov di, bx          ; DI copy rect.y
        mov cl, ds:[si+1]   ; CL = rect.w
        xor ch, ch          ; CX = width
        mov dl, ds:[si+2]   ; DL = rect.h
        xor dh, dh          ; DX = height
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
        popf                ; restore flags
    }
}

void mda_fill_rect(const mda_rect_t* rect, const mda_cell_t* cell) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        cld                 ; inc str ops
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, rect        ; DS:SI *rect
        lodsb               ; AL = rect.x
        xor ah, ah          ; AX = rect.x
        mov bl, ds:[si]     ; BL = rect.y
        xor bh, bh          ; BX = rect.y
        mov di, bx          ; DI copy rect.y
        mov cl, ds:[si+1]   ; CL = rect.w
        xor ch, ch          ; CX = width
        mov dl, ds:[si+2]   ; DL = rect.h
        xor dh, dh          ; DX = height
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
        // 6. draw horizontal lines length CX height times
 NEXT:  mov cx, bx          ; restore width
        rep stosw           ; draw hline
        add di, si          ; next line *VRAM + 160 - width
        dec dx
        jnz NEXT
        popf                ; restore flags
    }
}

// void mda_blit(mda_rect_t* to, mda_rect_t* from);

void mda_fill_screen(const mda_cell_t* cell) {
    __asm {
        // 1. register & flag setup
        pushf
        cld                 ; inc str ops
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        xor di, di
        lds si, cell        ; DS:SI *rect
        lodsw               ; AX = attribut:char pair
        mov cx, MDA_SCREEN_WORDS
        rep stosw           ; fill VRAM text page with cell
        popf                ; restore flags
    }
}

void mda_save_screen(const FILE* f) {
    require_fd(f, "NULL file pointer!");
    ensure(fwrite(MDA_VRAM_PTR, sizeof(char), MDA_SCREEN_BYTES, f) == MDA_SCREEN_BYTES, "FAIL to write!");
}

void mda_load_screen(const FILE* f) {
    require_fd(f, "NULL file pointer!");
    ensure(fread(MDA_VRAM_PTR, sizeof(char), MDA_SCREEN_BYTES, f) == MDA_SCREEN_BYTES, "FAIL to read!");
}

void mda_save_rect(const FILE* f, const mda_rect_t* rect) {
    require_fd(f, "NULL file pointer!");
    mda_cell_t* vram = mda_as_pointer(&rect->origin);
    for (int y = 0; y < rect->h; y++) {
        ensure(fwrite(vram, sizeof(mda_cell_t), rect->w, f) == rect->w, "FAIL to write!");
        vram += MDA_ROW_WORDS;
    }
}

void mda_load_rect(const FILE* f, const mda_rect_t* rect) {
    require_fd(f, "NULL file pointer!");
    mda_cell_t* vram = mda_as_pointer(&rect->origin);
    for (int y = 0; y < rect->h; y++) {
        ensure(fread(vram, sizeof(mda_cell_t), rect->w, f) == rect->w, "FAIL to read!");
        vram += MDA_ROW_WORDS;
    }
}

void mda_scroll_up(const mda_rect_t* rect, const mda_cell_t* blank) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        cld                 ; inc str ops
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, rect        ; DS:SI *rect
        lodsb               ; AL = rect.x
        xor ah, ah          ; AX = rect.x
        mov bl, ds:[si]     ; BL = rect.y
        xor bh, bh          ; BX = rect.y
        mov cl, ds:[si+1]   ; CL = rect.w
        xor ch, ch          ; CX = width
        mov dl, ds:[si+2]   ; DL = rect.h
        xor dh, dh          ; DX = height
        mov di, bx          ; DI copy rect.y
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
        // 3. register setup
        mov  ax, MDA_SEGMENT
        mov  ds, ax
        mov  si, di         ; DS:SI* source = ES:DI* destination
        mov  ax, MDA_ROW_BYTES
        add  si, ax         ; DS:SI* is now 1 line down
        sub  ax, cx         ; next line offset
        sub  ax, cx         ; 160 - (2 * width)
        // 4. move successive rows up 1
        dec  dx             ; height -1
        mov  bx, cx         ; BX copy width
NEXT:   rep  movsw          ; copy row cells upwards left to right
        mov  cx, bx         ; restore width counter
        add  si, ax         ; next line down
        add  di, ax
        dec  dx
        jnz  NEXT           ; loop until all rows moved up 1
        lds  si, blank
        lodsw               ; AX = blank attrib:char
        rep  stosw          ; bottom blank line
        popf                ; restore flags
    }
}

void mda_scroll_down(const mda_rect_t* rect, const mda_cell_t* blank) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, rect        ; DS:SI *rect
        lodsb               ; AL = rect.x
        xor ah, ah          ; AX = rect.x
        mov bl, ds:[si]     ; BL = rect.y
        xor bh, bh          ; BX = rect.y
        mov cl, ds:[si+1]   ; CL = rect.w
        xor ch, ch          ; CX = width
        mov dl, ds:[si+2]   ; DL = rect.h
        xor dh, dh          ; DX = height
        add ax, cx          ; move x to far right
        dec ax              ; AX = rect.x + rect.w - 1
        add bx, dx          ; move y to bottom
        dec bx              ; BX = rect.y + rect.h - 1
        mov di, bx          ; DI copy rect.y
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
        // 3. register setup
        mov  ax, MDA_SEGMENT
        mov  ds, ax
        mov  si, di         ; DS:SI* source = ES:DI* destination
        mov  ax, MDA_ROW_BYTES
        sub  si, ax         ; DS:SI* is now 1 line up
        sub  ax, cx         ; next line offset
        sub  ax, cx         ; 160 - (2 * width)
        // 4. move successive rows down 1
        dec  dx             ; height -1
        mov  bx, cx         ; BX copy width
        std                 ; decrement direction
NEXT:   rep  movsw          ; copy row cells downwards right to left
        mov  cx, bx         ; restore width counter
        sub  si, ax         ; next line up
        sub  di, ax
        dec  dx
        jnz  NEXT           ; loop until all rows moved up 1
        lds  si, blank
        lodsw               ; AX = blank attrib:char
        rep  stosw          ; top blank line
        popf                ; restore flags
    }
}

void mda_scroll_left(const mda_rect_t* rect, const mda_cell_t* blank) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI* VRAM
        lds si, rect        ; DS:SI* rect
        lodsb               ; AL = rect.x
        xor ah, ah          ; AX = rect.x
        mov bl, ds:[si]     ; BL = rect.y
        xor bh, bh          ; BX = rect.y
        mov cl, ds:[si+1]   ; CL = rect.w
        xor ch, ch          ; CX = width
        mov dl, ds:[si+2]   ; DL = rect.h
        xor dh, dh          ; DX = height
        mov di, bx          ; DI copy rect.y
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
        // 3. register setup
        lds  si, blank      ; DS:SI* blank
        mov  bx, ds:[si]    ; BX = blank char:attr
        mov  ax, MDA_SEGMENT
        mov  ds, ax
        mov  si, di         ; DS:SI* source = ES:DI* destination
        mov  ax, MDA_ROW_BYTES
        add  si, 2          ; DS:SI* is now 1 cell right
        dec  cx
        sub  ax, cx         ; next line offset
        sub  ax, cx         ; 160 - (2 * width)
        // 4. move successive rows left 1
        cld
        push bp             ; preserve BP it used to recover the return address for this function
        mov bp, cx          ; BP copy of width
NEXT:   rep  movsw          ; copy row cells left
        mov  cx, bp
        mov  es:[di], bx    ; blank end cell
        add  si, ax         ; next line down
        add  di, ax
        dec  dx
        jnz  NEXT           ; loop until all rows moved up 1
        pop bp              ; restore BP
        popf                ; restore flags
    }
}

void mda_scroll_right(const mda_rect_t* rect, const mda_cell_t* blank) {
    __asm {
        .8086
        // 1. register & flag setup
        pushf
        mov ax, MDA_SEGMENT
        mov es, ax          ; ES:DI *VRAM
        lds si, rect        ; DS:SI *rect
        lodsb               ; AL = rect.x
        xor ah, ah          ; AX = rect.x
        mov bl, ds:[si]     ; BL = rect.y
        xor bh, bh          ; BX = rect.y
        mov cl, ds:[si+1]   ; CL = rect.w
        xor ch, ch          ; CX = width
        mov dl, ds:[si+2]   ; DL = rect.h
        xor dh, dh          ; DX = height
        add ax, cx          ; move x to far right
        dec ax              ; AX = rect.x + rect.w - 1
        add bx, dx          ; move y to bottom
        dec bx              ; BX = rect.y + rect.h - 1
        mov di, bx          ; DI copy rect.y
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
        // 3. register setup
        lds  si, blank      ; DS:SI* blank
        mov  bx, ds:[si]    ; BX = blank char:attr
        mov  ax, MDA_SEGMENT
        mov  ds, ax
        mov  si, di         ; DS:SI* source = ES:DI* destination
        mov  ax, MDA_ROW_BYTES
        sub  si, 2          ; DS:SI* is now 1 cell left
        dec  cx
        sub  ax, cx         ; next line offset
        sub  ax, cx         ; 160 - (2 * width)
        // 4. move successive rows right 1
        std                 ; decrement direction
        push bp             ; preserve BP it used to recover the return address for this function
        mov bp, cx          ; BP copy of width
NEXT:   rep  movsw          ; copy row cells right
        mov  cx, bp         ; restore CX
        mov  es:[di], bx    ; blank end cell
        sub  si, ax         ; next line down
        sub  di, ax
        dec  dx
        jnz  NEXT           ; loop until all rows moved up 1
        pop bp              ; restore BP
        popf                ; restore flags
    }
}
