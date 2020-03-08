
#include "shivver/runtime.h"
#include "shivver/codec/ascii.h"

// Allocate a new parser state for a null-terminated string.
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

        return state;
}


// Free a parser state.
void    shivver_parse_free
        (parser_t* state)
{
        free(state);
}


// Load the next token into the peek buffer.
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


// Parse the given token, or error.
void    shivver_parse_tok
        (parser_t* state, size_t tok)
{
        shivver_parse_peek(state);
        if(state->peek_tok == tok)
        {       shivver_parse_shift(state);
                return;
        }

        printf("expected %s\n", shivver_token_name(tok));
        shivver_fail("unexpected tok");
}

