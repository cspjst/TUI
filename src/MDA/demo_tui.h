#ifndef DEMO_TUI_H
#define DEMO_TUI_H

#include "mda_cell.h"
#include "mda_primitives.h"
#include "mda_context.h"
#include "cp437_constants.h"
#include <stdio.h>

void demo_mda_ptr(mda_context_t* ctx) {
    mda_point_t p = mda_point_make(20, 10);
    mda_cell_t cell = mda_cell_make(CP437_SMILING_FACE, MDA_NORMAL | MDA_BLINK);
    mda_cell_t* vram = mda_as_pointer(&p);
    printf("VRAM = %p", vram);
    *vram = cell;
}

void demo_plot(mda_context_t* ctx) {
  mda_cell_t cell = mda_cell_make('*', ctx->attributes);
  mda_point_t p0 = mda_point_make(5, 2);
  mda_point_t p1 = mda_point_make(20, 10);
  printf("(%i, %i)", p0.x, p0.y);
  printf("(%i, %i)", p1.x, p1.y);
  mda_plot(&p0, &cell);
  mda_plot(&p1, &cell);
}

void demo_hline(mda_context_t* ctx) {
  mda_point_t p0 = mda_point_make(5, 2); // line of width 1
  mda_point_t p1 = mda_point_make(5, 2);
  mda_cell_t cell = mda_cell_make('-', ctx->attributes);

  for(int i = 0; i < 20; ++i) {
    mda_draw_hline(&p0, &p1, &cell);
    p0.y++;
    p1.x++;
  }
}

void demo_hline_cap(mda_context_t* ctx) {
  mda_point_t p0 = mda_point_make(5, 2); // line of width 1
  mda_point_t p1 = mda_point_make(5, 2);
   mda_cell_t cell = mda_cell_make('-', ctx->attributes);
  mda_cell_t cells[] = {0x071B, 0x072D, 0x071A}; // ←-→

  for(int i = 0; i < 20; ++i) {
    mda_draw_hline_caps(&p0, &p1, cells);
    p0.y++;
    p1.x++;
  }
}

void demo_vline(mda_context_t* ctx) {
  mda_point_t p0 = mda_point_make(5, 2); // line of height 1
  mda_point_t p1 = mda_point_make(5, 2);
  mda_cell_t cell = mda_cell_make('|', ctx->attributes);

  for(int i = 0; i < 20; ++i) {
    mda_draw_vline(&p0, &p1, &cell);
    p0.x += 2;
    p1.y++;
  }
}

void demo_vline_cap(mda_context_t* ctx) {
  mda_point_t p0 = mda_point_make(5, 2); // line of width 1
  mda_point_t p1 = mda_point_make(5, 2);
   mda_cell_t cell = mda_cell_make('-', ctx->attributes);
  mda_cell_t cells[] = {0x0718, 0x077C, 0x0719}; // ↑|↓

  for(int i = 0; i < 20; ++i) {
    mda_draw_vline_caps(&p0, &p1, cells);
    p0.x++;
    p1.y++;
  }
}

void demo_attr(mda_context_t* ctx) {
   mda_point_t p = mda_point_make(5, 2);
  // 1. Initialize in place
  mda_cell_t cell;
  mda_init_cell(&cell, 'A', ctx->attributes);
  mda_plot(&p, &cell);

  // 2. Modify attribute: OR in bold
  mda_cell_bold(&cell);
  p.x++;
  mda_plot(&p, &cell);

  // 3. Set new character
  mda_cell_set_char(&cell, 'B');
  p.x++;
  mda_plot(&p, &cell);

  // 4. Set underline (replaces current attr)
  mda_cell_set_attr(&cell, MDA_UNDERLINE);
  p.x++;
  mda_plot(&p, &cell);

  // 5. OR in blink
  mda_cell_blink(&cell);
  p.x++;
  mda_plot(&p, &cell);

  // 6. Use make_cell for temporary
  cell = mda_cell_make('C', MDA_REVERSE);
  p.x++;
  mda_plot(&p, &cell);

  // 7. Direct packed access
  mda_cell_set_char(&cell, 'D');
  mda_cell_set_attr(&cell, MDA_BOLD | MDA_BLINK);
  p.x++;
  mda_plot(&p, &cell);
}

void demo_draw_rect(mda_context_t* ctx) {
  mda_rect_t r = mda_rect_make(5, 2, 35, 7);
  mda_cell_t cell = mda_cell_make(CP437_DARK_SHADE, ctx->attributes);
  mda_draw_rect(&r, &cell);
  getchar();
  r = mda_rect_make(6, 3, 33, 5);
  cell = mda_cell_make(CP437_LIGHT_SHADE, ctx->attributes);
  mda_fill_rect(&r, &cell);
}

void demo_blit(mda_context_t* ctx) {

}

void demo_fill_screen(mda_context_t* ctx) {
    mda_cell_t cell = mda_cell_make(CP437_SMILING_FACE, MDA_NORMAL | MDA_BLINK);
    mda_fill_screen(&cell);
    getchar();
    mda_clear_screen();
}

void demo_save_restore(mda_context_t *ctx) {
    FILE* f;
    mda_cell_t cell = mda_cell_make(CP437_SMILING_FACE, MDA_NORMAL);

    f = fopen("screen.mda", "wb");
    require_fd(f, "FAIL to open file!");
    mda_fill_screen(&cell);
    mda_save_screen(f);
    fclose(f);
    printf("Screen saved to screen.mda\n");

    getchar();
    mda_clear_screen();

    printf("Press any key to restore...\n");
    getchar();

    f = fopen("screen.mda", "rb");
    require_fd(f, "FAIL to open file!");
    mda_load_screen(f);
    fclose(f);
    printf("Screen restored!\n");
}

void demo_rect_save_restore(mda_context_t *ctx) {
    FILE* f;
    mda_rect_t r0 = mda_rect_make(5, 2, 35, 7);
    mda_rect_t r1 = mda_rect_make(10, 15, 35, 7);
    mda_cell_t cell = mda_cell_make(CP437_SMILING_FACE, MDA_NORMAL);

    f = fopen("rect.mda", "wb");
    require_fd(f, "FAIL to open file!");
    mda_fill_screen(&cell);
    mda_save_rect(f, &r0);
    fclose(f);
    printf("Rectangle saved to rect.mda\n");

    getchar();
    mda_clear_screen();

    printf("Press any key to load...\n");
    getchar();

    f = fopen("rect.mda", "rb");
    require_fd(f, "FAIL to open file!");
    mda_load_rect(f, &r1);
    getchar();
    rewind(f);
    mda_load_rect(f, &r0);
    fclose(f);
}

void demo_scroll (mda_context_t *ctx) {
    mda_rect_t r0 = mda_rect_make(9, 14, 37, 9);
    mda_rect_t r1 = mda_rect_inner(&r0);
    mda_cell_t blank = mda_cell_make(' ', MDA_NORMAL);
    mda_cell_t test = mda_cell_make('*', MDA_NORMAL);

    mda_fill_screen(&test);
    FILE* f = fopen("RECT.MDA", "rb");
    require_fd(f, "FAIL to open file!");
    mda_load_rect(f, &r0);
    fclose(f);

    char k = getchar();
    while(k != 'q') {
        switch(k) {
            case 'w':
                mda_scroll_up(&r1, &blank);
                break;
            case 's':
                mda_scroll_down(&r1, &blank);
                break;
            case 'a':
                mda_scroll_left(&r1, &blank);
                break;
        };
        k = getchar();
    }
}

#endif
