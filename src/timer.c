#include "timer.h"
#include "display.h"
#include "buttons.h"
#include "utils.h"
#include "display_macros.h"
#include "typedefs.h"
#include <avr/io.h>
#include <avr/interrupt.h>

uint32_t elapsed_time;
BOOL enable_outputs = TRUE;
BOOL enable_elapse_time = FALSE; 

void init_clock(void){
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 16667;               // Set interval for 5 ms (16667 clocks @ 3.333 MHz)
    TCB1.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL_DIV2_gc;      // Enable
    sei();
}

/*
    clocked_outputs()
    Drives the outputs that are clock depedant
*/
void clocked_outputs(void){
    if(enable_outputs){
        
    }
}

ISR(TCB1_INT_vect)
{
    elapsed_time += 5; // Increment by 5ms

    multiplex_displays(); // Drive displays
    debounce(); // Debounce pushbuttons on clock
    clocked_input_handler(); // Clock dependant input handler

    if(enable_outputs){ clocked_outputs(); } // Clock dependant outputs (simons turn)

    TCB1.INTFLAGS = TCB_CAPT_bm;
}