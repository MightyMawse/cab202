#ifndef BUTTONS_H
#define BUTTONS_H

extern uint8_t pb1_state;
extern uint8_t pb2_state;
extern uint8_t pb3_state;
extern uint8_t pb4_state;
extern uint8_t pb_bm;
extern uint8_t pb_falling_edge;
extern uint8_t pb_rising_edge;

void init_buttons(void);
void debounce(void);
void clocked_input_handler(void);

#endif