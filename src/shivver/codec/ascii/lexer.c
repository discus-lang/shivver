
#include "shivver/codec/ascii.h"

// ------------------------------------------------------------------------------------------------
// Get the name of a token.
const char*
shivver_token_name
        (size_t tag)
{
        switch(tag)
        { case TOKEN_NONE:      return "none";
          case TOKEN_END:       return "end";
          case TOKEN_RBRA:      return "rbra";
          case TOKEN_RKET:      return "rket";
          case TOKEN_CBRA:      return "cbra";
          case TOKEN_CKET:      return "cket";
          case TOKEN_COMMA:     return "comma";

          case TOKEN_VAR:       return "var";
          case TOKEN_SYM:       return "sym";
          case TOKEN_PRM:       return "prm";
          default:              abort();
        }
}


// ------------------------------------------------------------------------------------------------
// Get the next token from the state.
//  On lex success:
//   The state is updated so the pos points to the character after the token.
//   The 'tok' output is set to the token tag.
//   The 'len' output is set to the length of the token.
//   To read long tokens, jump backwards 'len' bytes from the current position,
//   to get to the first character in that token.
//
//  On lex failure, returns false.
//   The current state should be inspected to get the source position of the failure.
//
void    shivver_lexer_next
        ( lexer_t* state        // current lexer state, which is updated.
        , size_t* tok           // output for token tag.
        , size_t* len)          // output for token length.
{
        // Current character.
        char c  = 0;

  // Try to lex a token from this point.
  again:
        // Save the current position as the token start.
        state->prev = state->pos;

        // If we're a the end of the string produce the special END token.
        if (state->pos >= state->len)
        {       *tok = TOKEN_END;
                *len = 0;
                return;
        }

        // The first character determines what sort of token this is.
        c = state->buf[state->pos];
        switch (c)
        { case ' ':  state->pos++; goto again;
          case '\t': state->pos++; goto again;
          case '\n': state->pos++; goto again;

          case '(': *tok = TOKEN_RBRA;  goto single;
          case ')': *tok = TOKEN_RKET;  goto single;
          case '{': *tok = TOKEN_CBRA;  goto single;
          case '}': *tok = TOKEN_CKET;  goto single;
          case ',': *tok = TOKEN_COMMA; goto single;

          // symbol
          case '%':
          {     *tok    = TOKEN_SYM;
                *len    = shivver_lexer_scan_symprm(state);
                return;
          }

          // primitive
          case '#':
          {     *tok    = TOKEN_PRM;
                *len    = shivver_lexer_scan_symprm(state);
                return;
          }

          default:
                // variables
                if (c >= 'a' && c <= 'z')
                {       *tok    = TOKEN_VAR;
                        *len    = shivver_lexer_scan_var(state);
                        return;
                }

                // lex failure.
                *tok    = TOKEN_NONE;
                *len    = 0;
                return;
        }

  // Single character token.
  single:
        *len = 1;
        state->pos++;
        return;

}


// ------------------------------------------------------------------------------------------------
// Scan a variable name, returning the raw length in the lex buffer.
size_t  shivver_lexer_scan_var
        (lexer_t* state)
{
        size_t i        = 0;
        char   c        = 0;

  again:
        c = state->buf[state->pos + i];
        if (c >= 'a' && c <= 'z')
        {       i++;
                goto again;
        }

        state->pos += i;
        return i;
}


// Load a variable name from the lex buffer into the given string buffer.
//   The state should be as it was just after scanning the token.
void    shivver_lexer_load_var
        ( lexer_t*      state
        , char*         out)
{
        size_t iOut     = 0;
        size_t iBuf     = state->prev;

        while (iBuf < state->pos)
                out[iOut++] = state->buf[iBuf++];

        out[iOut] = 0;
}


// ------------------------------------------------------------------------------------------------
// Scan a symbol name, returning the raw length in the lex buffer.
size_t  shivver_lexer_scan_symprm
        (lexer_t* state)
{
        size_t i        = 0;
        char   c        = 0;

        // skip over the starting '%' sigil.
        i++;

  again:
        c = state->buf[state->pos + i];
        if (c >= 'a' && c <= 'z')
        {       i++;
                goto again;
        }

        state->pos += i;
        return i;
}


// Load a symbol name from the lex buffer into the given string buffer.
//   The state should be as it was just after scanning the token.
void    shivver_lexer_load_symprm
        ( lexer_t*      state
        , char*         out)
{
        size_t iOut     = 0;
        size_t iBuf     = state->prev + 1;

        while (iBuf < state->pos)
                out[iOut++] = state->buf[iBuf++];

        out[iOut] = 0;
}
