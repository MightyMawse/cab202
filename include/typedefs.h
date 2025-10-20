#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdint.h>

typedef enum{
    TRUE = 1,
    FALSE = 0
} BOOL;

typedef enum {
    GENERATE,
    PROMPT,
    INPUT_WAITING,
    INPUT_RECEIVED,
    INPUT_EVALUATE,
    DISPLAY_SUCCESS,
    DISPLAY_FAILURE
} state_t;

#endif