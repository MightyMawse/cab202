#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

void lfsr_seed(uint32_t seed);
uint8_t lfsr_next(void);

#endif