#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdint.h>

typedef enum{
    TRUE = 1,
    FALSE = 0
} bool_t;

typedef enum {
    GENERATE,
    PROMPT,
    INPUT_WAITING,
    INPUT_RECEIVED,
    INPUT_EVALUATE,
    DISPLAY_SUCCESS,
    DISPLAY_FAILURE,
    DISPLAY_SCORE,
    SCORE_TABLE
} state_t;

#endif