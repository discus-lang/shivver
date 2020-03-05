
#include "shivver/runtime.h"
#include "shivver/codec/ascii.h"


// Parse an expression.
obj_t*
shivver_parse_term
        (lexer_t* state)
{
        size_t tag      = TOKEN_NONE;
        size_t len      = 0;

        shivver_lexer_next(state, &tag, &len);

        // Decide what to do base on the next token.
        switch (tag)
        { // Term ::= '(' Term ')'
          case TOKEN_RBRA:
          {     obj_t* oResult
                 = shivver_parse_term(state);
                shivver_parse_tok(state, TOKEN_RKET);
                return oResult;
          }

          // Term ::= Var
          case TOKEN_VAR:
                // Copy the name from the lexer buffer.
                return aVarH(len, state->buf + state->prev);

          // Term ::= Sym
          case TOKEN_SYM:
                // Skip over the sigil char when copying the name.
                return aSymH(len - 1, state->buf + state->prev + 1);

          // Term ::= Prm
          case TOKEN_PRM:
                // Skip of the sigil char when copying the name.
                return aPrmH(len - 1, state->buf + state->prev + 1);

          default:
                printf("parse fail\n");
                abort();
        }
}

// Parse the given token, or error.
void    shivver_parse_tok
        (lexer_t* state, size_t tok)
{
        size_t tag      = TOKEN_NONE;
        size_t len      = 0;

        shivver_lexer_next(state, &tag, &len);
        if (tag == tok) return;

        printf("expected %s\n", shivver_token_name(tok));
        abort();
}
