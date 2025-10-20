#include "display.h"
#include "display_macros.h"
#include "buzzer.h"
#include "typedefs.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

volatile uint8_t left_byte = DISP_OFF | DISP_LHS;
volatile uint8_t right_byte = DISP_OFF;

ISR(SPI0_INT_vect) {
    PORTA.OUTCLR = PIN1_bm;
    PORTA.OUTSET = PIN1_bm;
    SPI0.INTFLAGS = SPI_IF_bm;
}

/*
    init_spi()
    Initialise SPI
*/
void init_spi(void) {
    // Route SPI to correct pins
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;

    // SCK + MOSI as outputs
    PORTC.DIRSET = PIN0_bm | PIN2_bm;
    PORTC.OUTSET = PIN0_bm | PIN2_bm;

    // Latch pin as output
    PORTA.DIRSET = PIN1_bm;
    PORTA.OUTSET = PIN1_bm;

    // SPI master, clock /4, unbuffered
    SPI0.CTRLA = SPI_MASTER_bm;  // could also add clock select if needed
    SPI0.CTRLB = SPI_SSD_bm;
    SPI0.INTCTRL = SPI_IE_bm;
    SPI0.CTRLA |= SPI_ENABLE_bm; // enable SPI
}

/*
    multiplex_display()
    Alternate displays at high frequency
*/
void multiplex_displays(void){
    static uint8_t current_side = 0;
    if (current_side) {
        SPI0.DATA = left_byte;
    }
    else {
        SPI0.DATA = right_byte;
    }

    current_side = !current_side;
}

/*
    update_display()
    Set the bytes to be loaded into SPI.DATA
*/
void update_display(const uint8_t left, const uint8_t right)
{
    left_byte = (left | DISP_LHS);
    right_byte = right;
}