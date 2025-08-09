/**
 * @file mda_primitives.h
 * @brief Low-Level MDA Text-Mode Drawing Primitives
 * @details Provides unbounded, high-performance rendering functions
 * for points, lines, rectangles, borders, blits, and screen utilities
 * on MDA/Hercules monochrome displays.
 *
 * @note All functions are UNBOUNDED — caller must clip coordinates
 *       to 80x25 screen limits before calling.
 * @note Designed for direct video memory (0xB0000) access.
 * @author Jeremy Thornton
 */
#ifndef MDA_PRIMITIVES_H
#define MDA_PRIMITIVES_H

#include "mda_types.h"
#include <stdio.h>

/**
 * @defgroup drawing_primitives Drawing Primitives
 * @brief Core rendering operations: points, lines, rectangles, borders.
 * @note All functions assume caller has clipped input coordinates.
 * @{
 */
void mda_plot(mda_point_t* point, mda_cell_t* cell);

void mda_draw_hline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell);

void mda_draw_vline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell);

void mda_draw_hline_caps(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cells);

void mda_draw_vline_caps(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cells);

void mda_draw_rect(mda_rect_t* rect, mda_cell_t* cell);

void mda_fill_rect(mda_rect_t* rect, mda_cell_t* cell);

void mda_draw_border(mda_rect_t* rect, mda_cell_t* cells);

void mda_blit(mda_rect_t* to, mda_rect_t* from);
///@}

/**
 * @defgroup screen_ops Screen-Level Utilities
 * @brief High-level operations affecting full screen or large regions.
 * @{
 */
void mda_fill_screen(mda_cell_t* cell);  ///< Fill entire screen with given cell

/**
 * @brief Clear entire screen (black, invisible text).
 * @details Fills screen with null character and normal attribute.
 */
void mda_clear_screen(void);
///@}

/**
 * @defgroup rect_ops Rectangle Utilities
 * @brief Helper functions for rectangle initialization, save/load.
 * @{
 */
static inline void mda_clear_rect(mda_rect_t* rect) {
    mda_cell_t cell = mda_cell_make('\0', MDA_NORMAL);
    mda_fill_rect(rect, &cell);
} ///< Clear rectangle to null char + normal attr
///@}

/**
 * @defgroup rect_io Rectangle Save/Load
 * @brief Read/write rectangle contents from/to a binary stream.
 * Useful for testing, screen snapshots, and blit verification.
 * @{
 */

/**
 * @brief Save the contents of a rectangle to a binary stream.
 * @param rect Source rectangle (position ignored, size used).
 * @param f    Open binary FILE* for writing.
 * @note Format: consecutive cell_t pairs (char + attr) in row-major order.
 * @note Caller must ensure file is opened in binary mode.
 */
void mda_save_rect(mda_rect_t* rect, FILE* f);

/**
 * @brief Load contents from a binary stream into a rectangle.
 * @param rect Target rectangle (position ignored, size used).
 * @param f    Open binary FILE* for reading.
 * @note Expects consecutive cell_t pairs in row-major order.
 * @note Caller must ensure stream contains valid data of correct size.
 * @warning No bounds checking on input — use with trusted sources.
 */
void mda_load_rect(mda_rect_t* rect, FILE* f);
///@}

#endif /* MDA_PRIMITIVES_H */
