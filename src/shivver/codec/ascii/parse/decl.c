
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
          {     shivver_parse_shift(state);
                obj_t* oMac     = shivver_parse_mac(state);
                shivver_parse_tok(state, TOKEN_EQ);
                obj_t* oBody    = shivver_parse_term1(state);
                return  aApsH (2, aSymT("decl"), (obj_t*[]){oMac, oBody});
          }

          default:
          {
                //  Build an error message in freshly allocated space.
                //  The space will get freed along with the parse state.
                shivver_parse_fail
                        ( state, "Unexpected '%s'"
                        , shivver_token_name(state->peek_tok));
          }
        }
}
