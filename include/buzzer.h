#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

void init_buzzer();
void emit_frequency(uint8_t tone);
void stop_tone(void);

#endif
