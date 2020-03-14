
#include "shivver/codec/ascii.h"


// Parse a sequence of declarations.
obj_t*
shivver_parse_decls
        (parser_t* state)
{
        shivver_parse_peek(state);
        switch (state->peek_tok)
        { // END
          case TOKEN_END:
          {     return  aSymT ("list'nil");
          }

          // TODO: push multi decls into obj_list so we can have a single wrapper object.
          default:
          {     obj_t* oDecl  = shivver_parse_decl(state);
                obj_t* oDecls = shivver_parse_decls(state);
                return aApsH (2, aSymT("list'cons"), (obj_t*[]){oDecl, oDecls});
          }
        }
}


// Parse a declaration.
obj_t*
shivver_parse_decl
        (parser_t* state)
{
        shivver_parse_peek(state);
        switch (state->peek_tok)
        { // Term ::= Mac '=' Term
          case TOKEN_MAC:
          {     shivver_parse_shift(state);
                obj_t* oSym     = aSymA(state->curr_len - 1, state->curr_str + 1);
                shivver_parse_tok(state, TOKEN_EQ);
                obj_t* oBody    = shivver_parse_term1(state);
                shivver_parse_tok(state, TOKEN_SEMI);

                // We pack the macro name into a symbol so that the resulting
                // represenation of the declaration is inert.
                return  aApsH (2, aSymT("decl"), (obj_t*[]){oSym, oBody});
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
