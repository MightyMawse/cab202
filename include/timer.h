#ifndef TIMER_H
#define TIMER_H

#include "typedefs.h"

void init_clock(void);
void clocked_outputs(void);

extern uint32_t elapsed_time;
extern BOOL enable_outputs;

#endif
