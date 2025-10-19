#include <avr/interrupt.h>
#include <buttons.h>
#include <timer.h>
#include "display.h"
#include "display_macros.h"
#include "buzzer.h"
#include "buttons.h"

void state_machine(void);

static uint16_t sequenceLength = 1; 

int main(void)
{
    cli();

    // Call your initialisation functions here

    init_spi(); // Init spi
    init_clock(); // Init program clock
    init_buzzer();
    init_buttons(); // Init push buttons

    sei();

    // The program should not reach this point
    while (1)
        ;
}


void state_machine(void)
{
    while (1)
    {
        // Implement your main loop here
    }
}
