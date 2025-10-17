#include "buzzer.h"
#include <avr/io.h>

void init_buzzer(void) {

    // TCA0 will control the buzzer (PB0), so we need to enable it as an output
    PORTB.OUTCLR = PIN0_bm; // buzzer off initially
    PORTB.DIRSET = PIN0_bm; // Enable PB0 as output

    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;

    // Single-slope PWM mode, WO2 enable (PB5, LED DISP DP)    
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;

    // Enable overflow interrupt (interrupt at TOP)
    // TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;

    // PWM initially off
    TCA0.SINGLE.PER = 1;
    TCA0.SINGLE.CMP0 = 0;

    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;      // Enable TCA0 
}//buzzer_init

/*
    emit_frequency()
    Emits given frequency
*/
void emit_frequency(uint8_t tone){
    // TODO: Emit frequency
    // Frequencies: 330, 277, 440, 165
    // static const uint16_t per_values[4] = {10101, 12034, 7576, 20202};


    // Frequencies: 225, 189, 300, 112 Hz (using my student number of 75)

    // how to calculate period value

    // https://www.sensorsone.com/frequency-to-period-calculator/

    // frequency in hz

    // period in picoseconds

    // period / something = frequency

    static const uint16_t per_values[4] = {11110, 13227, 8332, 22320};

    TCA0.SINGLE.PERBUF = per_values[tone];
    TCA0.SINGLE.CMP0BUF = per_values[tone] >> 1;
}

void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
}//stop_tone