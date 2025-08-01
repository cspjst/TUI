#ifndef MDA_TYPES_H
#define MDA_TYPES_H

#include <stdint.h>

typedef union {
    uint16_t char_attr;
    struct {
        char chr;
        char attr;
    } parts;
} mda_char_attr_t;

typedef union {
  uint32_t packed;
  struct {
        uint8_t x;
        uint8_t y;
        uint8_t w;
        uint8_t h;
    } rect;
} mda_rect_t;

#endif
