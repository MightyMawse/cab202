#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "display_macros.h"

extern volatile uint8_t left_byte;
extern volatile uint8_t right_byte;

static const uint8_t seg_digits[10] = {
    0b00001000, // 0
    0b01101011, // 1
    0b01000100, // 2
    0b01000001, // 3
    0b00100011, // 4
    0b00010001, // 5
    0b00010000, // 6
    0b01001011, // 7
    0b00000000, // 8
    0b00000001  // 9
};

void update_display(const uint8_t left, const uint8_t right);
void init_spi(void);
void multiplex_displays(void);

#endif
