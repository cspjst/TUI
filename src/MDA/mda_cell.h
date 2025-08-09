/**
 * @file mda_cell.h
 * @brief MDA Character-Attribute Cell Utilities
 * @details Provides a union type for MDA/Hercules video memory cells
 * (character + attribute) and inline functions to manipulate them safely.
 * Designed for direct use with video memory at MDA_SEGMENT.
 * @author Jeremy Thornton
 */
#ifndef MDA_CELL_H
#define MDA_CELL_H

#include "../CONTRACT/contract.h"
#include "mda_attributes.h"
#include <stdint.h>

/**
 * @union mda_cell_t
 * @brief Represents a single character cell in MDA video memory.
 * @details Packed 16-bit value: low byte = character, high byte = attribute.
 * Layout matches direct memory access pattern at 0xB0000.
 */
typedef union {
    uint16_t packed;          /**< Access as raw 16-bit value for fast write */
    struct {
        char chr;             /**< Character code (ASCII) */
        uint8_t attr;         /**< Attribute byte (color/blink/underline/etc) */
    };                        /**< Anonymous struct for field access */
} mda_cell_t;

static inline void mda_cell_set_attr(mda_cell_t* cell, uint8_t attr) {
    require_address(cell, "NULL cell!");
    cell->attr = attr;       /**< Set full attribute byte */
}

static inline void mda_cell_or_attr(mda_cell_t* cell, uint8_t attr) {
    require_address(cell, "NULL cell!");
    cell->attr |= attr;      /**< OR in additional attribute bits (e.g. bold) */
}

static inline void mda_cell_set_char(mda_cell_t* cell, char chr) {
    require_address(cell, "NULL cell!");
    cell->chr = chr;         /**< Set character code only */
}

static inline void mda_init_cell(mda_cell_t* cell, char chr, uint8_t attr) {
    require_address(cell, "NULL cell!");
    cell->chr = chr;         /**< Initialize both character and attribute */
    cell->attr = attr;
}

static inline mda_cell_t mda_cell_make(char chr, uint8_t attr) {
    mda_cell_t c;
    mda_init_cell(&c, chr, attr);
    return c;                /**< Construct and return a new cell */
}

static inline void mda_cell_underline(mda_cell_t* cell) {
    require_address(cell, "NULL cell!");
    cell->attr = (cell->attr & 0xF8) | MDA_UNDERLINE;  /**< Clear low bits, set underline */
}

static inline void mda_cell_bold(mda_cell_t* cell) {
    mda_cell_or_attr(cell, MDA_BOLD);   /**< Add bold (intensity) bit */
}

static inline void mda_cell_blink(mda_cell_t* cell) {
    mda_cell_or_attr(cell, MDA_BLINK);  /**< Enable blinking */
}

static inline void mda_cell_reverse(mda_cell_t* cell) {
    mda_cell_set_attr(cell, MDA_REVERSE); /**< Full reverse video (black on white) */
}

#endif /* MDA_CELL_H */
