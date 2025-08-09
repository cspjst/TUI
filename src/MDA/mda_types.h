/**
 * @file mda_types.h
 * @brief Composite MDA Data Types for Text-Mode Display Programming
 * @details Aggregates the core structural types used in MDA/Hercules
 * console systems:
 *
 * - mda_point_t  : (x,y) cursor or origin positions
 * - mda_dim_t    : (w,h) region dimensions
 * - mda_rect_t   : bounding boxes via position + size
 * - mda_cell_t   : character-attribute pairs for video memory
 *
 * This header enables unified access to the MDA object model,
 * supporting screen layout, clipping, and cell manipulation
 * in 80x25 monochrome text mode.
 *
 * @note Designed for direct mapping to B000:0000 video memory.
 * @author Jeremy Thornton
 */

#ifndef MDA_TYPES_H
#define MDA_TYPES_H

#include "mda_point.h"
#include "mda_dim.h"
#include "mda_cell.h"
#include "mda_rect.h"

#endif /* MDA_TYPES_H */
