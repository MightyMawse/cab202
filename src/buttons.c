#include <avr/interrupt.h>
#include <avr/io.h>
#include <buttons.h>
#include "display.h"

#define MAX_COUNT 5
#define PB_NUM 4

uint8_t pb1_state = 0xFF;
uint8_t pb2_state = 0xFF;
uint8_t pb3_state = 0xFF;
uint8_t pb4_state = 0xFF;

// State referene array
uint8_t* p_state[4] = {
    &pb1_state, &pb2_state, 
    &pb3_state, &pb4_state
};

/*
    init_buttons()
    Enable pull up resistors, set interrupts
*/
void init_buttons(){
    cli();

    // Enable pull-up in S1, S2, S3, S4
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN7CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;

    sei();
}

/*
    PB (1-4) Interrupt service routine
*/
/*
ISR(PORTA_PORT_vect){
    static uint8_t counters[PB_NUM] = {0, 0, 0, 0}; // Array of counters for each button

    uint8_t pb_samples[4] = {
        (VPORTA.INTFLAGS & PIN4_bm),
        (VPORTA.INTFLAGS & PIN5_bm),
        (VPORTA.INTFLAGS & PIN6_bm),
        (VPORTA.INTFLAGS & PIN7_bm)
    };

    // Debounce each button
    for(uint8_t i = 0; i < PB_NUM; i++){ // Foreach pb
        uint8_t sample = pb_samples[i]; // Sample it
        
        if(sample ^ *p_state[i]){ // If change in sample vs state
            if(counters[i] + 1 < MAX_COUNT){ // Increment counter
                counters[i]++;
            }
            else{ 
                *p_state[i] = sample; // Update state
            }
        }
        else{
            counters[i] = 0; // Reset counter
        }
    }
}
*/