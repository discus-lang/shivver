
#include <assert.h>
#include <string.h>
#include "sv/source.h"


// Allocate a new parser state for the given input string.
sv_source_parser_t*
sv_source_parser_alloc(
        char* input)
{
        sv_source_parser_t* state
         = malloc(sizeof(sv_source_parser_t));
        memset(state, 0, sizeof(sv_source_parser_t));

        state->scanner.input            = input;
        state->scanner.length           = strlen(input);
        state->scanner.next             = 0;
        state->scanner.pos.line         = 1;
        state->scanner.pos.column       = 1;
        state->scanner.tab_columns      = 8;

        // Load the current and lookahead token into the state.
        //  If the input is empty these will both be end-of-input token.
        sv_source_parser_shift(state);
        sv_source_parser_shift(state);

        return state;
}


void
sv_source_parser_fail(
        sv_source_parser_t* state,
        char* foramt,
        ...)
{
        assert(false);
}


// Scan the next token and shift it into the state.
void
sv_source_parser_shift(
        sv_source_parser_t* state)
{
        state->here = state->next;
        assert(sv_token_scan(&state->scanner, &(state->next)));
}