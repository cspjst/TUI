#include <stdio.h>

#include "MDA/mda_attributes.h"
#include "MDA/mda_context.h"
#include "MDA/mda_cell.h"

int main() {
    printf(" TUI - mda_cell_t Interface Exercise\n");

    mda_context_t ctx;
    mda_cell_t cell;

    mda_initialize_default_context(&ctx);

    // 1. Initialize in place
    mda_init_cell(&cell, 'A', ctx.attributes);
    mda_plot(&ctx.bounds, 10, 5, cell.packed);

    // 2. Modify attribute: OR in bold
    mda_cell_bold(&cell);
    mda_plot(&ctx.bounds, 11, 5, cell.packed);

    // 3. Set new character
    mda_cell_set_char(&cell, 'B');
    mda_plot(&ctx.bounds, 12, 5, cell.packed);

    // 4. Set underline (replaces current attr)
    mda_cell_set_attr(&cell, MDA_UNDERLINE);
    mda_plot(&ctx.bounds, 13, 5, cell.packed);

    // 5. OR in blink
    mda_cell_blink(&cell);
    mda_plot(&ctx.bounds, 14, 5, cell.packed);

    // 6. Use make_cell for temporary
    cell = mda_make_cell('C', MDA_REVERSE);
    mda_plot(&ctx.bounds, 15, 5, cell.packed);

    // 7. Direct packed access
    mda_cell_set_char(&cell, 'D');
    mda_cell_set_attr(&cell, MDA_BOLD | MDA_BLINK);
    mda_plot(&ctx.bounds, 16, 5, cell.packed);

    return 0;
}
