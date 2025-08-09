/**
 * @file mda_attributes.h
 * @brief MDA/Hercules Text Attribute Constants
 * @details Defines attribute constants used for text display on IBM PC/XT, AT,
 * and compatible systems using Monochrome Display Adapter (MDA) or
 * Hercules Graphics Card (HGC) text modes.
 * @author Jeremy Thornton
 */
#ifndef MDA_ATTRIBUTES_H
#define MDA_ATTRIBUTES_H

/**
 * @enum mda_attribute_t
 * @brief Text attribute constants for MDA/Hercules monochrome displays.
 * @details These constants represent foreground/background intensity, blinking,
 * underlining, and reverse video modes. They are designed to be used
 * or combined (via bitwise OR) when writing to video memory.
 * @note Not all combinations are visually distinct on real hardware.
 *       For example, underline only appears on Hercules cards in text mode.
 */
typedef enum {
    MDA_NORMAL      = 0x07, /**< White on black – standard text */
    MDA_UNDERLINE   = 0x01, /**< Underlined text (Hercules only) */
    MDA_BOLD        = 0x08, /**< Bright white (intensity bit; OR with NORMAL) */
    MDA_BLINK       = 0x80, /**< Blinking text (applies to entire character) */
    MDA_REVERSE     = 0x70, /**< Black on white – reversed foreground/background */
    MDA_INVISIBLE   = 0x00  /**< Invisible text (fg == bg) – hidden content */
} mda_attribute_t;

#endif /* MDA_ATTRIBUTES_H */
