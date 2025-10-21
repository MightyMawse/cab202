#include <avr/interrupt.h>
#include <avr/io.h>
#include <buttons.h>
#include "display.h"
#include "buzzer.h"
#include "display_macros.h"

#define MAX_COUNT 10
#define PB_NUM 4

#define PB_MASK    (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)

static uint8_t pb_previous_state = 0xFF;   // last debounced snapshot
static uint8_t pb_current_state  = 0xFF;   // current debounced snapshot
uint8_t pb_debounced_state = 0xFF;  // working copy during debounce
uint8_t pb_falling_edge = 0x00;     // press (1→0)
uint8_t pb_rising_edge  = 0x00;     // release (0→1)
static uint8_t counters[PB_NUM] = {0};

/*
    init_buttons()
    Enable pull up resistors, set interrupts
*/
void init_buttons(void){
    cli();

    // Enable pull-up in S1, S2, S3, S4
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN7CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;

    sei();
}

/*
    debounce()
    Debounces inputs on clock
*/
void debounce(void){

   uint8_t raw = VPORTA.IN;  // read all pins at once (active low)

    // Debounce each button bit (4–7)
    for (uint8_t i = 0; i < PB_NUM; i++) {
        uint8_t mask = (1 << (i + 4));
        uint8_t sample = (raw & mask) ? 1 : 0; // active low → pressed = 0
        uint8_t stable = (pb_debounced_state & mask) ? 1 : 0;

        if (sample != stable) {
            if (++counters[i] >= MAX_COUNT) {
                counters[i] = 0;
                if (sample)
                    pb_debounced_state |= mask;   // released
                else
                    pb_debounced_state &= ~mask;  // pressed
            }
        } else {
            counters[i] = 0;
        }
    }

    // Edge detection between two consecutive debounced snapshots
    pb_previous_state = pb_current_state;
    pb_current_state  = pb_debounced_state;

    pb_falling_edge = pb_previous_state & ~pb_current_state; // press  (1→0)
    pb_rising_edge  = ~pb_previous_state & pb_current_state; // release (0→1)
}

/*
    clocked_input_handler()
    Clock driven input handler for pushbuttons
*/
void clocked_input_handler(void){
    

}

/*
    PB (1-4) Interrupt service routine
*/
ISR(PORTA_PORT_vect){
    PORTA.INTFLAGS = (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);
}