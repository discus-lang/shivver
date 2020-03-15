
#include "shivver/codec/ascii.h"


// Parse a sequence of declarations.
obj_t*
shivver_parse_decls
        (parser_t* state)
{
        objlist_t* list = shivver_objlist_alloc();
        shivver_parse_decls_list(state, list);
        return aApsH (list->used, aSymT("svr'decls"), list->list);
}

// Parse a sequence of declarations, appending them to the given list.
void
shivver_parse_decls_list
        ( parser_t*     state
        , objlist_t*    list)
{
        while(true) {
                shivver_parse_peek(state);
                switch (state->peek_tok)
                { // END
                  case TOKEN_END:
                  {     return;
                  }

                  // DECL DECLS
                  default:
                  {     obj_t* oDecl  = shivver_parse_decl(state);
                        shivver_objlist_append(list, oDecl);
                  }
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
        { // Term ::= '!def' Mac '=' Term
          case TOKEN_KEY_DEF:
          {     shivver_parse_shift(state);
                obj_t* oMac     = shivver_parse_mac(state);
                shivver_parse_tok(state, TOKEN_EQ);
                obj_t* oBody    = shivver_parse_term1(state);

                // We pack the macro name into a symbol so that the resulting
                // represenation of the declaration is inert.
                return  aApsH (2, aSymT("svr'decl"), (obj_t*[]){oMac, oBody});
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
