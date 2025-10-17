#include "display.h"
#include "display_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

volatile uint8_t left_byte = DISP_OFF | DISP_LHS;
volatile uint8_t right_byte = DISP_OFF;

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

ISR(SPI0_INT_vect) {
    PORTA.OUTCLR = PIN1_bm;
    PORTA.OUTSET = PIN1_bm;
    //uint8_t dummy = SPI0.DATA;  // clears the flag properly
    SPI0.INTFLAGS = SPI_IF_bm;
}

void update_display(const uint8_t left, const uint8_t right)
{
    left_byte = (left | DISP_LHS);
    right_byte = right;
}