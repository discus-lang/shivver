
#include <setjmp.h>
#include "shivver/heap.h"
#include "shivver/codec/ascii.h"


// Allocate a new parser state to parse the given null-terminated string.
parser_t*
shivver_parse_alloc
        (char* str)
{
        parser_t* state = malloc(sizeof(parser_t));
        state->str      = str;
        state->len      = strlen(str);

        state->curr_tok = TOKEN_NONE;
        state->curr_str = 0;
        state->curr_len = 0;

        state->peek_tok = TOKEN_NONE;
        state->peek_str = 0;
        state->peek_len = 0;

        state->error_str = 0;

        return state;
}


// Free a parser state.
void    shivver_parse_free
        (parser_t* state)
{
        if (state->error_str != 0)
                free(state->error_str);
        free(state);
}


// Load the next token into the peek buffer of the state.
void    shivver_parse_peek
        (parser_t* state)
{
        // If already have a peeked then we don't need to scan another one.
        if (state->peek_tok != 0)
                return;

        // Scan the next token from the input.
        shivver_lexer_scan
                ( state->str, state->len
                , &state->peek_tok, &state->peek_str, &state->peek_len);
}


// Accept the peeked token.
//  This moves it from the 'peek' buffer to the 'curr' buffer,
//  and clears the 'peek' buffer.
void    shivver_parse_shift
        (parser_t* state)
{
        // There needs to be a peeked token already in the buffer.
        assert (state->peek_tok != 0);

        // Load the peeked token into the current token buffer.
        state->curr_tok = state->peek_tok;
        state->curr_str = state->peek_str;
        state->curr_len = state->peek_len;

        // Clear the peek buffer.
        state->peek_tok = 0;
        state->peek_str = 0;
        state->peek_len = 0;

        // Update the current position in the input string.
        state->str      = state->curr_str + state->curr_len;

        state->len      = state->len
                        - (state->curr_str - state->str)
                        - state->curr_len;
}


// Parse the given token, which must be returned next by the lexer,
// else fail with an unexpected token message.
void    shivver_parse_tok
        (parser_t* state, size_t tok)
{
        shivver_parse_peek(state);

        // We got the token that we were expecting.
        if(state->peek_tok == tok)
        {       shivver_parse_shift(state);
                return;
        }

        // We didn't get the token that we were expecting.
        //  Build an error message in freshly allocated space.
        //  The space will get freed along with the parse state.
        char* err = malloc(256);
        snprintf( err, 256
                , "Unexpected token %s"
                , shivver_token_name(tok));
        state->error_str = err;

        // Return to the caller of the top-level parse function.
        longjmp(state->jmp_err, 1);
}

