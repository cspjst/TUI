#ifndef DEMO_TUI_H
#define DEMO_TUI_H

#include "mda_context.h"
#include "cp437_constants.h"

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
  mda_cell_t cells[] = {0x071B, 0x07C4, 0x071A};

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

#endif
