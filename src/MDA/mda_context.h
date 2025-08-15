/**
 * @file mda_context.h
 * @brief MDA Display Context Management
 * @details Provides a stateful context structure for managing
 * MDA/Hercules text-mode display properties, including bounds,
 * attributes, cursor state, and tab settings. Integrates with
 * BIOS video services for hardware-level control.
 *
 * This context serves as the core runtime environment for
 * console-like I/O operations on monochrome displays.
 *
 * @author Jeremy Thornton
 */
#ifndef MDA_CONTEXT_H
#define MDA_CONTEXT_H

#include "mda_constants.h"
#include "mda_types.h"
#include "../BIOS/bios_video_services.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


/**
 * @struct mda_context_t
 * @brief Runtime context for MDA text-mode operations.
 *
 * Encapsulates display bounds, text attributes, tab settings,
 * and BIOS-level video and cursor state. Used to maintain
 * consistent console behavior across drawing and I/O functions.
 */
typedef struct {
    mda_rect_t bounds;           /**< Bounding rectangle for current context */
    char attributes;             /**< Current text attribute byte */
    mda_cell_t blank;            /**< Character:Attribute pair used for blank */
    uint8_t htab_size;           /**< Horizontal tab spacing (in columns) */
    uint8_t vtab_size;           /**< Vertical tab spacing (in rows) */
    bios_video_state_t video;    /**< Saved BIOS video mode and page info */
    bios_cursor_state_t cursor;  /**< Saved cursor position and shape */
    // TODO clip function here
    // TODO: mouse_state mouse; has mouse support etc
} mda_context_t;


/**
 * @brief Initialize context with default values (full screen, normal attributes).
 * @param ctx Pointer to context to initialize.
 */
void mda_initialize_default_context(mda_context_t* ctx);


/**
 * @brief Set the active bounds for the context.
 * @param ctx Pointer to context.
 * @param x   Left edge of bounds (column).
 * @param y   Top edge of bounds (row).
 * @param w   Width in columns.
 * @param h   Height in rows.
 */
void mda_set_bounds(mda_context_t* ctx, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/**
 * @defgroup cursor_ops Cursor Movement & Advancement
 * @brief Functions for direct and incremental cursor control.
 * @{
 */
void mda_cursor_to(mda_context_t* ctx, mda_point_t* p); ///<  Move cursor to specified position within bounds
void mda_cursor_advance(mda_context_t* ctx);  ///< Advance cursor right with wrap
///@}


/**
 * @defgroup ascii_controls ASCII Control Code Handlers
 * @brief Implements standard 7-bit ASCII control character behavior.
 * Each function processes one control code using current context state.
 * Designed to mirror classic terminal semantics on MDA/Hercules hardware.
 * @{
 */
void mda_BEL(const mda_context_t* ctx);  ///< Sound bell (CRTL-G)
void mda_BS(const mda_context_t* ctx);   ///< Backspace: move left, no underflow
void mda_HT(const mda_context_t* ctx);   ///< Horizontal tab: advance to next HT stop
void mda_LF(const mda_context_t* ctx);   ///< Line Feed: move down, scroll if needed
void mda_VT(const mda_context_t* ctx);   ///< Vertical Tab: advance down by vtab_size
void mda_FF(const mda_context_t* ctx);   ///< Form Feed: clear screen, home cursor
void mda_CR(const mda_context_t* ctx);   ///< Carriage Return: move to start of line
void mda_ESC(const mda_context_t* ctx);  ///< Escape: begin control sequence (stub)
/**
 * @brief Handle ASCII DEL — overwrite with invisible character.
 * @details Unlike BS, DEL does not move cursor left.
 * Instead, it writes an MDA_INVISIBLE cell (attribute 0x00).
 * This matches hardware behavior on MDA/Hercules.
 */
void mda_DEL(const mda_context_t* ctx);
///@}


/**
 * @defgroup text_output Text Output & Line Termination
 * @brief Character writing and standard line-ending sequences.
 * @{
 */
void mda_CRLF(const mda_context_t* ctx);
void mda_print_char(const mda_context_t* ctx, char chr);
void mda_print_string(const mda_context_t* ctx, char* str);
///@}

#endif /* MDA_CONTEXT_H */
