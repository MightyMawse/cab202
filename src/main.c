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
void display_sequence_element(uint8_t index);
void toggle_output(bool_t b);

static uint16_t sequence_length = 1; 
static state_t state = GENERATE;
static bool_t enable_output = FALSE;

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
    state_machine();

    // The program should not reach this point
    while (1)
        ;
}

/*
    toggle_output()
    Toggle output (also turns off output)
*/
void toggle_output(bool_t b){
    enable_output = b;
    if(b == FALSE){
        update_display(DISP_OFF, DISP_OFF);
        buzzer_stop();
    }
}

/*
    display_sequence_element()
    Display a column and emit frequency
*/
void display_sequence_element(uint8_t index){
    if(enable_output == FALSE)
        return;

    switch (index)
    {
    case 1:
        update_display(DISP_BAR_RIGHT, DISP_OFF);
        buzzer_emit(1);
        break;
    case 2:
        update_display(DISP_OFF, DISP_BAR_LEFT);
        buzzer_emit(2);
        break;
    case 3:
        update_display(DISP_OFF, DISP_BAR_RIGHT);
        buzzer_emit(3);
        break;
    default:
        update_display(DISP_BAR_LEFT, DISP_OFF);
        buzzer_emit(0);
        break;
    }
}


void state_machine(void)
{
    static uint8_t sequence_counter = 1; // Keep track of how far through the sequence we are
    while (1)
    {
        // Implement your main loop here
        switch (state)
        {
        case GENERATE:
            elapsed_time = 0;
            uint8_t rnd = lfsr_next();

            toggle_elapse(TRUE); // Start counting elapse time
            toggle_output(TRUE); // Enable outputs

            display_sequence_element(rnd); // Display the rnd column on segment and buzzer
            state = PROMPT;

            break;
        case PROMPT:
            if(elapsed_time >= playback_delay){
                toggle_elapse(FALSE); // Stop counting elapsed time
                toggle_output(FALSE); // Shutoff outputs

                if(sequence_counter == sequence_length){
                    sequence_counter = 0;
                    lfsr_seed(PRNG_SEED); // Reset seed back to start
                    state = INPUT_WAITING; // Start listening for inputs
                }
                else{
                    sequence_counter++;
                    state = GENERATE;
                }
            }
            break;
        case INPUT_WAITING:
            if(pb_falling_edge & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)){
                uint8_t pb_index = button_index(pb_falling_edge); // Find index of button pushed (Experimental)
                toggle_output(TRUE);
                display_sequence_element(pb_index);
                state = INPUT_RECEIVED;
            }
            break;
        case INPUT_RECEIVED:
            if(pb_rising_edge){
                
            }
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
