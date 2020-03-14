
#include "shivver/codec/ascii.h"


// Parse a declaration.
obj_t*
shivver_parse_decl
        (parser_t* state)
{
        shivver_parse_peek(state);

        switch (state->peek_tok)
        { // Term ::= !term Mac '=' Term
          case TOKEN_KEY_TERM:
          {     printf("parsing term decl\n");
                abort();
          }

          default:
          {
                //  Build an error message in freshly allocated space.
                //  The space will get freed along with the parse state.
                char* err = malloc(256);
                snprintf( err, 256
                        , "Unexpected token '%s'"
                        , shivver_token_name(state->peek_tok));
                state->error_str = err;
                longjmp(state->jmp_err, 1);
          }
        }
}
