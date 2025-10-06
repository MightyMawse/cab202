#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void init_clock(){
    // TCB0 is a 16-bit timer that has been configured to generate
    // an interrupt every 64th of a second. This was done to increase
    // the resolution of the timer.
    TCB0.CCMP = 52083;
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_ENABLE_bm;
}