
#include "shivver/codec/ascii.h"


// Check if the given token can start a term.
bool
shivver_parse_isTermStart(size_t tok)
{
        switch(tok)
        { case TOKEN_RBRA:
          case TOKEN_SBRA:
          case TOKEN_CBRA:
          case TOKEN_VAR:
          case TOKEN_SYM:
          case TOKEN_PRM:
                return true;
        }
        return false;
}


// Parse a term or term application.
obj_t*
shivver_parse_term
        (parser_t* state)
{
        obj_t* oFun     = shivver_parse_term0(state);

        shivver_parse_peek(state);

        // When the function is directly applied to a term list,
        // build an aps node directly to avoid the intermediate mmm node.
        if (state->peek_tok == TOKEN_SBRA)
        {       shivver_parse_shift(state);
                objlist_t* list = shivver_parse_termCommaList(state);
                shivver_parse_tok(state, TOKEN_SKET);
                obj_t* obj      = aApsH (list->used, oFun, list->list);
                shivver_objlist_free(list);
                return obj;
        }

        // Function is applied to some other term.
        if (shivver_parse_isTermStart(state->peek_tok))
        {       obj_t*  oArg    = shivver_parse_term0(state);
                return aAppH(oFun, oArg);
        }

        // Some other thing is after the first term.
        return oFun;
}


// Parse a term.
obj_t*
shivver_parse_term0
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

          // Term ::= '{' Name* '}' Term
          case TOKEN_CBRA:
          {     shivver_parse_shift(state);
                objlist_t* list = shivver_parse_varSpaceList(state);
                shivver_parse_tok(state, TOKEN_CKET);
                obj_t* objBody  = shivver_parse_term(state);
                obj_t* obj      = aAbsH(list->used, list->list, objBody);
                shivver_objlist_free(list);
                return obj;
          }

          // Term ::= '[' Term,* ']'
          case TOKEN_SBRA:
          {     shivver_parse_shift(state);
                objlist_t* list = shivver_parse_termCommaList(state);
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

// Parse a list of variable names separated by whitespace.
objlist_t*
shivver_parse_varSpaceList
        (parser_t* state)
{
        objlist_t* list = shivver_objlist_alloc();

  again:
        shivver_parse_peek(state);
        if (! (state->peek_tok == TOKEN_VAR))
                return list;

        shivver_parse_shift(state);
        obj_t* obj = aVarH(state->curr_len, state->curr_str);
        shivver_objlist_append(list, obj);
        goto again;
}


// Parse a list of terms separated by commas.
objlist_t*
shivver_parse_termCommaList
        (parser_t* state)
{
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