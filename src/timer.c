#include "timer.h"
#include "display.h"
#include "buttons.h"
#include "utils.h"
#include "display_macros.h"
#include "typedefs.h"
#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

uint32_t elapsed_time;
bool_t enable_outputs = TRUE;
bool_t enable_elapse_time = FALSE;
bool_t enable_elapse_clock = FALSE;
uint16_t playback_delay = 250; // 250ms playback delay NOTE: Change later for POT
uint8_t half_playback_delay = 0;

/*
    init_clock()
    Initialise the clock
*/
void init_clock(void){
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 3333;               // Set interval for 1 ms (3333 clocks @ 3.333 MHz)
    TCB1.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL_DIV2_gc;      // Enable
    sei();
}

/*
    update_playback_delay()
    Scale the playback delay with POT
*/
void update_playback_delay(){
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;

    while(!(ADC0.INTFLAGS & ADC_RESRDY_bm));

    playback_delay = 250 + ((ADC0.RESULT * 1750) >> 8);
    half_playback_delay = (playback_delay / 2);
    ADC0.INTFLAGS = ADC_RESRDY_bm;
}

/*
    toggle_elapse()
    toggle the elapse time counter
*/
void toggle_elapse(bool_t b){
    enable_elapse_clock = b;
}

ISR(TCB1_INT_vect)
{
    if(enable_elapse_clock == TRUE){
        elapsed_time += 2; // Increment by 5ms
    }

    multiplex_displays(); // Drive displays

    if(enable_input){
        debounce(); // Debounce pushbuttons on clock
    }

    TCB1.INTFLAGS = TCB_CAPT_bm;
}