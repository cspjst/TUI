#include <stdio.h>

#include "MDA/mda_context.h"
#include "MDA/mda_cell.h"
#include "MDA/mda_point.h"

int main() {
    printf(" TUI - mda_cell_t Interface Exercise\n");

    mda_context_t ctx;
    mda_initialize_default_context(&ctx);

    mda_cell_t cell = mda_make_cell('*', ctx.attributes);
    mda_point_t p0 = mda_make_point(5, 2);
    mda_point_t p1 = mda_make_point(5, 2);

    //mda_plot(p0.packed, cell.packed);

    for(int i = 0; i < 20; ++i) {
        mda_vline(p0.packed,p1.packed, cell.packed);
        p0.pos.x += 2;
        p1.pos.y++;
    }

    getchar();

    return 0;
}


/*
for(int i = 0; i < 20; ++i) {
    mda_hline(p0.packed,p1.packed, cell.packed);
    p0.pos.y++;
    p1.pos.x++;
}
 */

/*
// 1. Initialize in place
mda_init_cell(&cell, 'A', ctx.attributes);
mda_plot(p.packed, cell.packed);

// 2. Modify attribute: OR in bold
mda_cell_bold(&cell);
p.pos.x++;
mda_plot(p.packed, cell.packed);

// 3. Set new character
mda_cell_set_char(&cell, 'B');
p.pos.x++;
mda_plot(p.packed, cell.packed);

// 4. Set underline (replaces current attr)
mda_cell_set_attr(&cell, MDA_UNDERLINE);
p.pos.x++;
mda_plot(p.packed, cell.packed);

// 5. OR in blink
mda_cell_blink(&cell);
p.pos.x++;
mda_plot(p.packed, cell.packed);

// 6. Use make_cell for temporary
cell = mda_make_cell('C', MDA_REVERSE);
p.pos.x++;
mda_plot(p.packed, cell.packed);

// 7. Direct packed access
mda_cell_set_char(&cell, 'D');
mda_cell_set_attr(&cell, MDA_BOLD | MDA_BLINK);
p.pos.x++;
mda_plot(p.packed, cell.packed);
 */
