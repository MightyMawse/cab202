#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

#define E_HIGH_PER 4939
#define CS_PER 5893
#define A_PER 3708
#define E_LOW_PER 8333

void init_buzzer();
void buzzer_emit(uint8_t index);
void buzzer_stop(void);

#endif
