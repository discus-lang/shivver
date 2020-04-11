
#include "shivver/codec/ascii.h"


// Parse a sequence of declarations.
obj_t*
shivver_parse_decls
        (parser_t* state)
{
        objlist_t* list = shivver_objlist_alloc();
        shivver_parse_decls_list(state, list);
        obj_t* oDecls   = aApsH (list->used, aSymT("svr'decls"), list->list);
        shivver_objlist_free(list);
        return oDecls;
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
          // Term ::= '!def' Mac '{' Name* '}' '=' Term
          case TOKEN_KEY_DEF:
          {     shivver_parse_shift(state);
                obj_t* oMac     = shivver_parse_mac(state);

                shivver_parse_peek(state);
                switch (state->peek_tok)
                { // ... '=' Term
                  case TOKEN_EQ:
                  {     shivver_parse_shift(state);
                        obj_t* oBody    = shivver_parse_term1(state);
                        return aApsH (2, aSymT("svr'decl"), (obj_t*[]){oMac, oBody});
                  }

                  // ... '{' Name* '}' '=' Term
                  case TOKEN_CBRA:
                  {     shivver_parse_shift(state);
                        objlist_t* list = shivver_parse_varSpaceList(state);
                        shivver_parse_tok(state, TOKEN_CKET);
                        shivver_parse_tok(state, TOKEN_EQ);
                        obj_t* oBody    = shivver_parse_term1(state);
                        obj_t* oAbs     = aAbsH(list->used, list->list, oBody);
                        shivver_objlist_free(list);
                        return aApsH (2, aSymT("svr'decl"), (obj_t*[]){oMac, oAbs});
                  }

                  default:
                        shivver_parse_fail
                         ( state, "Unexpected '%s', expected '=' or decl parameters."
                         , shivver_token_name(state->peek_tok));
                }
          }

          default:
                shivver_parse_fail
                 ( state, "Unexpected '%s', expected declaration start."
                 , shivver_token_name(state->peek_tok));
        }
}
