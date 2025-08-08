#include <stdio.h>

#include "MDA/mda_context.h"
#include "MDA/demo_tui.h"

int main() {

    #ifndef __LARGE__
        printf("Incorrect memory model is selected.\n");
        printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
        return 0;
    #endif

    printf("DEMO - Text User Interface (TUI)\n");

    mda_context_t ctx;
    mda_initialize_default_context(&ctx);

    //demo_plot(&ctx);
    //demo_attr(&ctx);
    //demo_hline(&ctx);
    demo_hline_cap(&ctx);
    //demo_vline(&ctx);
    //demo_draw_rect(&ctx);

    getchar();

    return 0;
}
