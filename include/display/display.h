#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

typedef struct display_hw * display_hw_t;

display_hw_t display_init(uint8_t rs, uint8_t rw, uint8_t e, uint8_t d0, uint8_t d1,
        uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

void display_putc(const display_hw_t display, char c);

#endif
