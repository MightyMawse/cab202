#include "timer.h"
#include "display.h"
#include "buttons.h"
#include "display_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void init_clock(){
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 16667;               // Set interval for 5 ms (16667 clocks @ 3.333 MHz)
    TCB1.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL_DIV2_gc;      // Enable
    sei();
}

ISR(TCB1_INT_vect)
{
    // Flip flop displays
    static uint8_t current_side = 0;
    if (current_side) {
        SPI0.DATA = left_byte;
    }
    else {
        SPI0.DATA = right_byte;
    }

    current_side = !current_side;

    debounce(); // Debounce pushbuttons on clock
    clocked_input_handler(); // Clock dependant input handler

    TCB1.INTFLAGS = TCB_CAPT_bm;
}