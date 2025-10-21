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

#define DISP_1 0
#define DISP_2 1
#define DISP_3 2
#define DISP_4 3
#define DISP_CRCT 4
#define DISP_INCT 5
#define DISP_BLNK 6

void state_machine(void);
void display_sequence_element(uint8_t index);
void toggle_output(bool_t b);

static uint16_t sequence_length = 1; 
static state_t state = GENERATE;
static bool_t enable_output = FALSE;
static uint8_t sequence_counter = 1; // Keep track of how far through the sequence we are
static uint8_t input = 0;

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
    case DISP_2:
        update_display(DISP_BAR_RIGHT, DISP_OFF);
        buzzer_emit(1);
        break;
    case DISP_3:
        update_display(DISP_OFF, DISP_BAR_LEFT);
        buzzer_emit(2);
        break;
    case DISP_4:
        update_display(DISP_OFF, DISP_BAR_RIGHT);
        buzzer_emit(3);
        break;
    case DISP_CRCT:
        update_display(DISP_ON, DISP_ON); // Turn all on
        buzzer_emit(0);
        break;
    case DISP_INCT:
        update_display(DISP_DASH, DISP_DASH);
        buzzer_emit(3);
        break;
    case DISP_BLNK:
        update_display(DISP_OFF, DISP_OFF); // All off
        break;
    default:
        update_display(DISP_BAR_LEFT, DISP_OFF);
        buzzer_emit(0);
        break;
    }
}


void state_machine(void)
{
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
                    sequence_counter = 1;
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
            if(pb_falling_edge & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)){ // On push down
                if (pb_falling_edge & PIN4_bm) {
                    input = 0;
                } else if (pb_falling_edge & PIN5_bm) {
                    input = 1;
                } else if (pb_falling_edge & PIN6_bm) {
                    input = 2;
                } else if (pb_falling_edge & PIN7_bm) {
                    input = 3;
                }

                toggle_output(TRUE);
                display_sequence_element(input);
                state = INPUT_RECEIVED;
            }
            break;
        case INPUT_RECEIVED:
            if(pb_rising_edge & (1 << (input + 4))){ // On button lift
                toggle_output(FALSE);
                state = INPUT_EVALUATE;
            }
            break;
        case INPUT_EVALUATE:
            elapsed_time = 0;

            if(input == lfsr_next()){ // Seed has been reset, lets check
                if(sequence_counter == sequence_length){
                    toggle_output(TRUE); // Only display if we are at the end of the sequence
                    display_sequence_element(DISP_CRCT); // Display correct

                    toggle_elapse(TRUE); // Start counting

                    sequence_counter = 1;
                    lfsr_seed(PRNG_SEED); // Reset seed

                    state = DISPLAY_SUCCESS;
                }
                else{
                    sequence_counter++;
                    state = INPUT_WAITING;
                }
            }
            else{
                toggle_output(TRUE); // Only display if we are at the end of the sequence
                display_sequence_element(DISP_INCT); // Display incorrect

                toggle_elapse(TRUE); // Start counting

                sequence_counter = 1;
                sequence_length = 1;
                lfsr_seed(PRNG_SEED); // Reset seed

                state = DISPLAY_FAILURE;
            }
            break;
        case DISPLAY_SUCCESS:
            if(elapsed_time >= playback_delay){
                toggle_output(FALSE); // Turn off
                toggle_elapse(FALSE);

                elapsed_time = 0;
                sequence_length++;
                state = GENERATE;
            }
            break;
        case DISPLAY_FAILURE:
            if(elapsed_time >= playback_delay){
                toggle_output(FALSE); // Turn off
                toggle_elapse(FALSE);

                elapsed_time = 0;
                state = GENERATE;
            }
            break;
        default:
            state = GENERATE;
            //lfsr_seed(PRNG_SEED);
            break;
        }
    }
}
