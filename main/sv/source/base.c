
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "sv/source.h"


// Allocate a new parser state for the given input string.
sv_source_parse_t*
sv_source_parse_alloc(
        char* input)
{
        sv_source_parse_t* state
         = malloc(sizeof(sv_source_parse_t));
        memset(state, 0, sizeof(sv_source_parse_t));

        state->scanner.input            = input;
        state->scanner.length           = strlen(input);
        state->scanner.next             = 0;
        state->scanner.pos.line         = 1;
        state->scanner.pos.column       = 1;
        state->scanner.tab_columns      = 8;

        // Load the current and lookahead token into the state.
        //  If the input is empty these will both be end-of-input token.
        sv_source_parse_shift(state);
        sv_source_parse_shift(state);

        return state;
}


void
sv_source_parse_fail(
        sv_source_parse_t* state,
        char* format,
        ...)
{
        va_list args;
        va_start(args, format);
        char* result = 0;
        assert(vasprintf(&result, format, args) >= 0);
        va_end(args);

        printf("%s", result);
        exit(1);
}


// Scan the next token and shift it into the state.
void
sv_source_parse_shift(
        sv_source_parse_t* state)
{
        state->here = state->next;
        assert(sv_token_scan(&state->scanner, &(state->next)));
}