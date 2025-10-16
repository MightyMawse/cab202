#include <avr/interrupt.h>
#include <buttons.h>
#include <timer.h>
#include "display.h"

void state_machine(void);
void test_display_column();

uint16_t sequenceLength = 1; 

int main(void)
{
    cli();

    // Call your initialisation functions here

    init_buttons(); // Init push buttons
    init_clock(); // Init program clock
    init_spi(); // Init spi

    sei();

    state_machine();

    test_display_column();

    // The program should not reach this point
    while (1)
        ;
}

void test_display_column(){
    update_display(0b0000000, 0b0000000);
}

void state_machine(void)
{
    while (1)
    {
        // Implement your main loop here
    }
}
