#include <stdint.h>
#include <stdio.h>
#include "display.h"

#define MASK 0xE2025CAB

static uint32_t state_lfsr;

/*
    button_index()
    Find button pushed index
*/
uint8_t button_index(uint8_t bp_bm){
    for(uint8_t i = 0; i < sizeof(bp_bm); i++){
        if((bp_bm << i) == 0){
            return (i - 4);
        }
    }
    return 0;
}

/*
    get_segment_bits()
    Given a number, return the segments to display
*/
void get_segment_bits(int score, uint8_t* tens, uint8_t* ones){
    if (score < 0) score = 0;
    if (score > 99) score = 99;

    int t = score / 10;
    int o = score % 10;

    *tens = (t == 0) ? 0xFF : seg_digits[t]; // blank tens if zero
    *ones = seg_digits[o];
}

void lfsr_seed(uint32_t seed){
    state_lfsr = seed;
}

uint8_t lfsr_next(void){
    uint8_t bit = state_lfsr & 1;
    state_lfsr >>= 1;
    if(bit){
        state_lfsr ^= MASK;
    }
    return (uint8_t)(state_lfsr & 0b11);
}
