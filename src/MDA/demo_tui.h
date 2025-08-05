#ifndef DEMO_TUI_H
#define DEMO_TUI_H

void demo_plot(mda_context_t* ctx) {
  mda_cell_t cell = mda_cell_make('*', ctx.attributes);
  mda_point_t p0 = mda_point_make(5, 2);
  mda_point_t p1 = mda_point_make(20, 10);
  printf("(%i, %i)", p0.pos.x, p0.pos.y);
  printf("(%i, %i)", p1.pos.x, p1.pos.y);
  mda_plot(&p0, &cell);
  mda_plot(&p1, &cell);
}

void demo_hline(mda_context_t* ctx) {
  mda_point_t p0 = mda_point_make(5, 2); // line of width 1
  mda_point_t p1 = mda_point_make(5, 2);
  mda_cell_t cell = mda_cell_make('-', ctx.attributes);
  
  for(int i = 0; i < 20; ++i) {
    mda_hline(&p0, &p1, &cell);
    p0.pos.y++;
    p1.pos.x++;
  }
}

void demo_vline(mda_context_t ctx) {
  mda_point_t p0 = mda_point_make(5, 2); // line of height 1
  mda_point_t p1 = mda_point_make(5, 2);
  mda_cell_t cell = mda_cell_make('|', ctx.attributes);
  
  for(int i = 0; i < 20; ++i) {
    mda_vline(&p0, &p1, &cell);
    p0.pos.x += 2;
    p1.pos.y++;
  }
}

void demo_attr(mda_context_t ctx) {
   mda_point_t p = mda_point_make(5, 2);
  // 1. Initialize in place
  mda_cell_t cell;
  mda_init_cell(&cell, 'A', ctx.attributes);
  mda_plot(&p, &cell);
  
  // 2. Modify attribute: OR in bold
  mda_cell_bold(&cell);
  p.pos.x++;
  mda_plot(&p, &cell);
  
  // 3. Set new character
  mda_cell_set_char(&cell, 'B');
  p.pos.x++;
  mda_plot(&p, &cell);
  
  // 4. Set underline (replaces current attr)
  mda_cell_set_attr(&cell, MDA_UNDERLINE);
  p.pos.x++;
  mda_plot(&p, &cell);
  
  // 5. OR in blink
  mda_cell_blink(&cell);
  p.pos.x++;
  mda_plot(&p, &cell);
  
  // 6. Use make_cell for temporary
  cell = mda_make_cell('C', MDA_REVERSE);
  p.pos.x++;
  mda_plot(&p, &cell);
  
  // 7. Direct packed access
  mda_cell_set_char(&cell, 'D');
  mda_cell_set_attr(&cell, MDA_BOLD | MDA_BLINK);
  p.pos.x++;
  mda_plot(&p, &cell);
}

void demo_draw_rect(mda_context_t ctx) {
  mda_rect_t r = mda_rect_make(1, 2, 3, 4);
  mda_cell_t cell = mda_cell_make('*', ctx.attributes);

  printf("(x=%i, y=%i, w=%i, h=i%\n)\n", r.rect.x, r.rect.y, r.rect.w, r.rect.h,);
  mda_draw_rect(&r, &cell);
}

#endif
