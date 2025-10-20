#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

extern volatile uint8_t left_byte;
extern volatile uint8_t right_byte;

void update_display(const uint8_t left, const uint8_t right);
void init_spi(void);
void multiplex_displays(void);

#endif
