#include "buzzer.h"
#include <avr/io.h>

/*
    init_buzzer()
    Initialise buzzer
*/
void init_buzzer(void) {
    // TCA0 will control the buzzer (PB0), so we need to enable it as an output
    PORTB.OUTCLR = PIN0_bm; // buzzer off initially
    PORTB.DIRSET = PIN0_bm; // Enable PB0 as output

    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;

    // Single-slope PWM mode, WO2 enable (PB5, LED DISP DP)    
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;

    // PWM initially off
    TCA0.SINGLE.PER = 1;
    TCA0.SINGLE.CMP0 = 0;

    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;      // Enable TCA0 
}

/*
    buzzer_emit()
    Emits given frequency
*/
void buzzer_emit(uint8_t index){
    static const uint32_t per_values[4] = {
        E_HIGH_PER * 2,
        CS_PER * 2,
        A_PER * 2,
        E_LOW_PER * 2
    };

    TCA0.SINGLE.PERBUF = per_values[index];
    TCA0.SINGLE.CMP0BUF = per_values[index] >> 1;
}

/*
    buzzer_stop
    Stop buzzer frequency
*/
void buzzer_stop(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
}