
#include "shivver/codec/ascii.h"


// Check if the given token can start a term.
bool
shivver_parse_isTermStart(size_t tok)
{
        switch(tok)
        { case TOKEN_RBRA:
          case TOKEN_SBRA:
          case TOKEN_VAR:
          case TOKEN_SYM:
          case TOKEN_PRM:
                return true;
        }
        return false;
}


// Parse a term.
obj_t*
shivver_parse_term
        (parser_t* state)
{
        shivver_parse_peek(state);

        switch (state->peek_tok)
        { // Term ::= Var
          case TOKEN_VAR:
          {     // Copy the name from the lexer buffer.
                shivver_parse_shift(state);
                obj_t* obj = aVarH(state->curr_len, state->curr_str);
                return obj;
          }

          // Term ::= Sym
          case TOKEN_SYM:
          {     // Skip over the sigil char when copying the name.
                shivver_parse_shift(state);
                obj_t* obj = aSymH(state->curr_len - 1, state->curr_str + 1);
                return obj;
          }

          // Term ::= Prm
          case TOKEN_PRM:
          {     // Skip of the sigil char when copying the name.
                shivver_parse_shift(state);
                obj_t* obj = aPrmH(state->curr_len - 1, state->curr_str + 1);
                return obj;
          }

          // Term ::= '[' Term,* ']'
          case TOKEN_SBRA:
          {     shivver_parse_shift(state);
                objlist_t* list = shivver_parse_termList(state);
                shivver_parse_tok(state, TOKEN_SKET);
                obj_t* obj      = aMmmH(list->used, list->list);
                shivver_objlist_free(list);
                return obj;
          }

          // Term ::= '(' Term ')'
          case TOKEN_RBRA:
          {     shivver_parse_shift(state);
                obj_t* oResult  = shivver_parse_term(state);
                shivver_parse_tok(state, TOKEN_RKET);
                return oResult;
          }

          default:
                printf("parse fail\n");
                abort();
        }
}



// Parse a list of terms separated by commas.
objlist_t*
shivver_parse_termList
        (parser_t* state)
{
        // A new empty objlist.
        objlist_t* list = shivver_objlist_alloc();

  again:
        shivver_parse_peek(state);
        if (! shivver_parse_isTermStart(state->peek_tok))
                return list;

        obj_t* obj = shivver_parse_term(state);
        shivver_objlist_append(list, obj);

        shivver_parse_peek(state);
        if (state->peek_tok == TOKEN_COMMA)
        {       shivver_parse_shift(state);
                goto again;
        }

        return list;
}