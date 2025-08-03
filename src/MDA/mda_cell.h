#ifndef MDA_CELL_H
#define MDA_CELL_H

#include "../CONTRACT/contract.h"
#include "mda_attributes.h"
#include <stdint.h>

typedef union {
    uint16_t packed;
    struct {
        char chr;
        char attr;
    } parts;
} mda_cell_t;

static inline void mda_cell_set_attr(mda_cell_t* cell, uint8_t attr) {
    require_address(cell, "NULL cell!");
    cell->parts.attr = attr;
}

static inline void mda_cell_or_attr(mda_cell_t* cell, uint8_t attr) {
    require_address(cell, "NULL cell!");
    cell->parts.attr |= attr;
}

static inline void mda_cell_set_char(mda_cell_t* cell, char chr) {
    require_address(cell, "NULL cell!");
    cell->parts.chr = chr;
}

static inline void mda_init_cell(mda_cell_t* cell, char chr, uint8_t attr) {
    require_address(cell, "NULL cell!");
    cell->parts.chr = chr;
    cell->parts.attr = attr;
}

static inline mda_cell_t mda_make_cell(char chr, uint8_t attr) {
    mda_cell_t c;
    mda_init_cell(&c, chr, attr);
    return c;
}

static inline void mda_cell_underline(mda_cell_t* cell) {
    require_address(cell, "NULL cell!");
    cell->parts.attr = (cell->parts.attr & 0xF8) | MDA_UNDERLINE; // Ensure only bit 0-2 + underline
}

static inline void mda_cell_bold(mda_cell_t* cell) {
    mda_cell_or_attr(cell, MDA_BOLD);
}

static inline void mda_cell_blink(mda_cell_t* cell) {
    mda_cell_or_attr(cell, MDA_BLINK);
}

static inline void mda_cell_reverse(mda_cell_t* cell) {
    mda_cell_set_attr(cell, MDA_REVERSE);
}

#endif
