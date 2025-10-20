#include <stdint.h>
#include <stdio.h>

#define MASK 0xE2025CAB

static uint32_t state_lfsr;

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
