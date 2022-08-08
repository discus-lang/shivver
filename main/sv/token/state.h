#pragma once
#include "sv/token.h"

// State of the tokenizer.
typedef struct {
        // Input string.
        char*   input;

        // Max length of input to consider.
        size_t  length;

        // Index to next character to consider.
        size_t  next;

        // Source position of next token start.
        sv_token_pos_t pos;

        // How many columns to count for the tab character.
        size_t  tab_columns;
} sv_token_state_t;



