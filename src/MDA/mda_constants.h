/**
 * @file mda_constants.h
 * @brief MDA/Hercules Display Hardware Constants
 * @details Defines memory layout and display geometry constants for
 * IBM Monochrome Display Adapter (MDA) and Hercules Graphics Card (HGC) text modes.
 * @author Jeremy Thornton
 */
#ifndef MDA_CONSTANTS_H
#define MDA_CONSTANTS_H

#define MDA_SEGMENT      0B000h     /**< Base segment address for MDA video memory (use in __asm) */
#define MDA_COLUMNS      80         /**< Number of text columns per row */
#define MDA_ROWS         25         /**< Number of text rows per screen */
#define MDA_ROW_BYTES    160        /**< Bytes per row: 80 cols × 2 (char + attr) */
#define MDA_DEFAULT_HTAB 4          /**< Default horizontal tab spacing (columns) */
#define MDA_DEFAULT_VTAB 2          /**< Default vertical tab spacing (rows) */

#endif /* MDA_CONSTANTS_H */
