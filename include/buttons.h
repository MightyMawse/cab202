#ifndef BUTTONS_H
#define BUTTONS_H

#include "typedefs.h"

extern uint8_t pb_falling_edge;
extern uint8_t pb_rising_edge;
extern uint8_t pb_debounced_state;
extern bool_t enable_input;

void init_buttons(void);
void debounce(void);
void toggle_input(bool_t b);

#endif