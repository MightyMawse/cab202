#include "timer.h"
#include "display.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void init_clock(){
    cli();
    TCB0.CCMP = 52083;
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_ENABLE_bm;
    sei();
}

ISR(TCB0_INT_vect)
{
    static uint8_t current_side = 0;
    if (current_side) {
        SPI0.DATA = left_byte;
    }
    else {
        SPI0.DATA = right_byte;
    }
    current_side = !current_side;

    TCB1.INTFLAGS = TCB_CAPT_bm;
}