#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

void lfsr_seed(uint32_t seed);
uint8_t lfsr_next(void);
uint8_t button_index(uint8_t bp_bm);
void get_segment_bits(int score, uint8_t* tens, uint8_t* ones);

#endif