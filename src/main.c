#include <stdio.h>

#include "MDA/mda_context.h"
#include "MDA/demo_tui.h"

int main() {
    printf("DEMO - Text User Interface (TUI)\n");

    mda_context_t ctx;
    mda_initialize_default_context(&ctx);

    demo_plot(&ctx);

    getchar();

    return 0;
}
