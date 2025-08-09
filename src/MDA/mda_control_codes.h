/**
 * @file ascii_control_codes.h
 * @brief ASCII Control Codes for Text-Mode Terminal Processing
 * @details Defines standard 7-bit ASCII control characters used in
 * console I/O, cursor control, and terminal communication for MDA/Hercules
 * and compatible text-mode systems.
 * @author Jeremy Thornton
 */

#ifndef ASCII_CONTROL_CODES_H
#define ASCII_CONTROL_CODES_H

#define ASCII_NUL  0x00 /**< Null character – padding or string terminator */
#define ASCII_BEL  0x07 /**< Bell – triggers speaker (CRTL-G) */
#define ASCII_BS   0x08 /**< Backspace – move cursor left */
#define ASCII_HT   0x09 /**< Horizontal Tab – advance to next tab stop */
#define ASCII_LF   0x0A /**< Line Feed – move cursor down (and scroll) */
#define ASCII_VT   0x0B /**< Vertical Tab – move cursor down by tab */
#define ASCII_FF   0x0C /**< Form Feed – clear screen (MDA/Hercules) */
#define ASCII_CR   0x0D /**< Carriage Return – move cursor to start of line */
#define ASCII_SO   0x0E /**< Shift Out – switch to alternate character set */
#define ASCII_SI   0x0F /**< Shift In – return to standard character set */
#define ASCII_ESC  0x1B /**< Escape – start of control sequences (e.g. ANSI) */
#define ASCII_DEL  0x7F /**< Delete – high bit set; often used as rubout */

#endif /* ASCII_CONTROL_CODES_H */
