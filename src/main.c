#include <avr/interrupt.h>
#include <buttons.h>
#include <timer.h>
#include "display.h"
#include "display_macros.h"
#include "typedefs.h"
#include "buzzer.h"
#include "buttons.h"
#include "utils.h"
#include "adc.h"

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
static uint8_t score = 0;
static uint8_t high_score = 0;
static uint8_t last_score = 0;

int main(void)
{
    cli();

    // Call your initialisation functions here

    init_spi(); // Init spi
    init_clock(); // Init program clock
    init_buzzer();
    init_buttons(); // Init push buttons
    init_adc(); // Init potentiometer

    sei();

    lfsr_seed(PRNG_SEED, 0);
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

    update_playback_delay();

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
        break;
    case DISP_INCT:
        update_display(DISP_DASH, DISP_DASH);
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
            if(elapsed_time >= (playback_delay / 2)){
                toggle_output(FALSE); // Shutoff outputs

                if(sequence_counter == sequence_length){
                    if(elapsed_time >= playback_delay){ // Stay off for half playback delay
                        sequence_counter = 1;
                        elapsed_time = 0;

                        toggle_elapse(FALSE); // Stop counting elapsed time
                        lfsr_seed(PRNG_SEED, last_score); // Reset seed back to start

                        state = INPUT_WAITING; // Start listening for inputs
                    }
                }
                else{
                    if(elapsed_time >= playback_delay){ // Stay off for half playback delay
                        sequence_counter++;
                        state = GENERATE;
                    }
                }
            }
            break;
        case INPUT_WAITING:
            toggle_input(TRUE);

            if(pb_falling_edge & 0xF0){ // On push down
                uint8_t btn_bits = (pb_falling_edge & 0xF0) >> 4;

                switch (btn_bits) {
                    case 0x01: input = 0; break;
                    case 0x02: input = 1; break;
                    case 0x04: input = 2; break;
                    case 0x08: input = 3; break;
                }

                toggle_output(TRUE);
                toggle_elapse(TRUE);

                elapsed_time = 0;

                display_sequence_element(input);
                state = INPUT_RECEIVED;
            }
            break;
        case INPUT_RECEIVED:
            if(pb_rising_edge & (1 << (input + 4))){ // On button lift
                if(elapsed_time >= (playback_delay / 2)){ // Wait for elapse
                    toggle_output(FALSE);
                    state = INPUT_EVALUATE;
                }
            }
            else if(pb_debounced_state & (1 << (input + 4))){ // Check if button not pushed down
                if(elapsed_time >= (playback_delay / 2)){ // Wait for elapse
                    toggle_output(FALSE);
                    state = INPUT_EVALUATE;
                }
            }
            break;
        case INPUT_EVALUATE:
            elapsed_time = 0;

            toggle_input(FALSE);

            if(input == lfsr_next()){ // Seed has been reset, lets check
                if(sequence_counter == sequence_length){
                    toggle_output(TRUE); // Only display if we are at the end of the sequence
                    display_sequence_element(DISP_CRCT); // Display correct

                    sequence_counter = 1;
                    lfsr_seed(PRNG_SEED, last_score); // Reset seed

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

                sequence_counter = 1;
                sequence_length = 1;

                lfsr_seed(PRNG_SEED, last_score); // Reset seed

                state = DISPLAY_FAILURE;
            }
            break;
        case DISPLAY_SUCCESS:
            if(elapsed_time >= playback_delay){
                toggle_output(FALSE); // Turn off
                toggle_elapse(FALSE);

                elapsed_time = 0;
                sequence_length++;
                score++;
                state = GENERATE;
            }
            break;
        case DISPLAY_FAILURE:
            if(elapsed_time >= playback_delay){
                if(score > high_score){
                    high_score = score;

                    // Display score (no buzz)
                    uint8_t t, o;
                    get_segment_bits(high_score, &t, &o);
                    update_display(t, o);

                    elapsed_time = 0; // Reset elapsed time but dont disable it, for the score display
                    state = DISPLAY_SCORE;
                }
                else{
                    toggle_output(FALSE); // Turn off
                    toggle_elapse(FALSE);

                    state = GENERATE;
                }

                last_score = score + 1;
                lfsr_seed(PRNG_SEED, last_score);
                score = 0; // Reset score
            }
            break;
        case DISPLAY_SCORE:
            if(elapsed_time >= playback_delay){
                toggle_output(FALSE);
                elapsed_time = 0; // Again, reset time but dont disable
                state = SCORE_TABLE;
            }
            break;
        case SCORE_TABLE:
            if(elapsed_time >= playback_delay){ // Wait 5s for score board
                toggle_elapse(FALSE);
                elapsed_time = 0;
                state = GENERATE;
            }
            break;
        default:
            lfsr_seed(PRNG_SEED, last_score);
            state = GENERATE;
            break;
        }
    }
}
