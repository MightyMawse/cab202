#ifndef TIMER_H
#define TIMER_H

#include "typedefs.h"

void init_clock(void);
void toggle_elapse(bool_t b);

extern uint32_t elapsed_time;
extern uint16_t playback_delay;

#endif
