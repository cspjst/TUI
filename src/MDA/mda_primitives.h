/**
 * @file mda_primitives.h
 * @brief Low-Level MDA Text-Mode Drawing Primitives
 * @details Provides unbounded, high-performance rendering functions
 * for points, lines, rectangles, borders, and blits on MDA/Hercules
 * monochrome displays.
 *
 * @note All functions are UNBOUNDED — caller must clip coordinates
 *       to 80x25 screen limits before calling.
 * @note Designed for direct video memory (0xB0000) access.
 * @author Jeremy Thornton
 */
#ifndef MDA_PRIMITIVES_H
#define MDA_PRIMITIVES_H

#include "mda_types.h"

/**
 * @brief Plot a single cell at the given point.
 * @param point Pointer to the target position (x,y).
 * @param cell  Pointer to the cell (char + attr) to write.
 * @note Unbounded — ensure point is within 0..79 x 0..24.
 */
void mda_plot(mda_point_t* point, mda_cell_t* cell);

/**
 * @brief Draw a horizontal line (inclusive from p0 to p1).
 * @param p0  Start point.
 * @param p1  End point.
 * @param cell Cell to fill with.
 * @note Unbounded — caller must ensure y is valid and x in range.
 */
void mda_draw_hline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell);

/**
 * @brief Draw a vertical line (inclusive from p0 to p1).
 * @param p0  Start point.
 * @param p1  End point.
 * @param cell Cell to fill with.
 * @note Unbounded — caller must ensure x is valid and y in range.
 */
void mda_draw_vline(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cell);


/**
 * @brief Draw a horizontal line with end-cap cells.
 * @param p0    Start point.
 * @param p1    End point.
 * @param cells Pointer to 3 cells: [left cap, line body, right cap].
 * @note Unbounded — clipping required by caller.
 */
void mda_draw_hline_caps(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cells);

/**
 * @brief Draw a vertical line with end-cap cells.
 * @param p0    Start point.
 * @param p1    End point.
 * @param cells Pointer to 3 cells: [top cap, line body, bottom cap].
 * @note Unbounded — clipping required by caller.
 */
void mda_draw_vline_caps(mda_point_t* p0, mda_point_t* p1, mda_cell_t* cells);

/**
 * @brief Draw a rectangle outline (hollow).
 * @param rect  Pointer to bounding rectangle.
 * @param cell  Cell to draw edges with.
 * @note Unbounded — caller must clip rect to screen.
 */
void mda_draw_rect(mda_rect_t* rect, mda_cell_t* cell);

/**
 * @brief Fill a rectangle (solid).
 * @param rect  Pointer to bounding rectangle.
 * @param cell  Cell to fill with.
 * @note Unbounded — caller must clip rect to screen.
 */
void mda_fill_rect(mda_rect_t* rect, mda_cell_t* cell);

/**
 * @brief Draw a full border using 8 distinct cell types.
 * @param rect  Pointer to bounding rectangle.
 * @param cells Pointer to 8 cells in order:
 *              [tl, t, tr, l, r, bl, b, br]
 * @note Designed for box-drawing (e.g., UI frames).
 * @note Unbounded — caller must clip.
 */
void mda_draw_border(mda_rect_t* rect, mda_cell_t* cells);

/**
 * @brief Copy (blit) a rectangular region from one area to another.
 * @param to    Destination rectangle (defines size and location).
 * @param from  Source rectangle (position only — size from 'to').
 * @note Performs raw copy: no blending, no transparency.
 * @note Unbounded — caller must ensure both regions are in bounds.
 */
void mda_blit(mda_rect_t* to, mda_rect_t* from);

#endif /* MDA_PRIMITIVES_H */
