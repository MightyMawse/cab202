#include <avr/interrupt.h>
#include <buttons.h>
#include <timer.h>
#include "display.h"
#include "display_macros.h"
#include "typedefs.h"
#include "buzzer.h"
#include "buttons.h"
#include "utils.h"

#define PRNG_SEED 0x12024945

void state_machine(void);

static uint16_t sequence_length = 1; 
static state_t state = GENERATE;

int main(void)
{
    cli();

    // Call your initialisation functions here

    init_spi(); // Init spi
    init_clock(); // Init program clock
    init_buzzer();
    init_buttons(); // Init push buttons

    sei();
    
    lfsr_seed(PRNG_SEED);

    // The program should not reach this point
    while (1)
        ;
}

/*
    enable_outputs()
    Enables clocked outputs
*/
void enable_outputs(void){

}


void state_machine(void)
{
    static uint8_t input_counter = 0;
    while (1)
    {
        // Implement your main loop here
        switch (state)
        {
        case GENERATE:
            
            break;
        case PROMPT: // Simons turn
            for(uint8_t i = 0; i < sequence_length; i++){
                uint8_t rng = lfsr_next();
                
            }
            break;
        case INPUT_WAITING:
            break;
        case INPUT_RECEIVED:
            break;
        case INPUT_EVALUATE:
            break;
        case DISPLAY_SUCCESS:
            break;
        case DISPLAY_FAILURE:
            break;
        default:
            break;
        }
    }
}
