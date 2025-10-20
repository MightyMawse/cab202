#ifndef BUTTONS_H
#define BUTTONS_H

extern uint8_t pb1_state;
extern uint8_t pb2_state;
extern uint8_t pb3_state;
extern uint8_t pb4_state;

void init_buttons(void);
void debounce(void);
void input(void);
void clocked_input_handler(void);

#endif