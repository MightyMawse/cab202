#ifndef BUTTONS_H
#define BUTTONS_H

extern uint8_t pb_falling_edge;
extern uint8_t pb_rising_edge;
extern uint8_t pb_debounced_state;

void init_buttons(void);
void debounce(void);
void clocked_input_handler(void);

#endif