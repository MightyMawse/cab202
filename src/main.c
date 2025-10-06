#include <avr/interrupt.h>
#include <buttons.h>
#include <timer.h>

void state_machine(void);

uint16_t sequenceLength = 1; 

int main(void)
{
    cli();

    // Call your initialisation functions here

    init_buttons(); // Init push buttons
    init_clock(); // Init program clock

    sei();

    state_machine();

    // The program should not reach this point
    while (1)
        ;
}

void test_display_column(uint8_t seg){
    
}

void state_machine(void)
{
    while (1)
    {
        // Implement your main loop here
    }
}
