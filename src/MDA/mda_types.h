#ifndef MDA_TYPES_H
#define MDA_TYPES_H

#include <stdint.h>

typedef union {
    uint16_t packed;
    struct {
        char chr;
        char attr;
    } parts;
} mda_cell_t;

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
