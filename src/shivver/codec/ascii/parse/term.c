
#include <setjmp.h>
#include "shivver/codec/ascii.h"


// Parse a term.
//  On success, the term is returned directly.
//  On failure, we return 0 and the error message is in state->error_str.
//   The message string in state->error_str needs to be freed by the caller.
obj_t*
shivver_parse_term_zero
        (parser_t* state)
{
        int ret = setjmp(state->jmp_err);
        if (ret == 0)
                return shivver_parse_term1 (state);
        else {
                // We got longjmped to, so there was a parse error.
                // An error message need to have been set in the state.
                assert(state->error_str != 0);

                // Signal to the caller that there was an error by
                // returning 0.
                return 0;
        }
}


// Parse a term or term application.
obj_t*
shivver_parse_term1
        (parser_t* state)
{
        obj_t* oFun     = shivver_parse_term0 (state);

        // See if the first term is followed by a second one.
        shivver_parse_peek(state);

        // When the function is directly applied to a term list,
        // then build an aps node rather than an app with an
        // itermediate mmm node.
        if (state->peek_tok == TOKEN_SBRA)
        {       shivver_parse_shift(state);
                objlist_t* list = shivver_parse_termCommaList(state);
                shivver_parse_tok(state, TOKEN_SKET);
                obj_t* obj      = aApsH (list->used, oFun, list->list);
                shivver_objlist_free(list);
                return obj;
        }

        // Function is applied to some other term,
        // so build a regular app node.
        if (shivver_parse_isTermStart(state->peek_tok))
        {       obj_t*  oArg    = shivver_parse_term0(state);
                return aAppH(oFun, oArg);
        }

        // Some other thing follows the first term.
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
                obj_t* obj = aVarA(state->curr_len, state->curr_str);
                return obj;
          }

          // Term ::= Mac
          case TOKEN_MAC:
          {     // Skip over the sigil char when copying the name.
                shivver_parse_shift(state);
                obj_t* obj = aMacA(state->curr_len - 1, state->curr_str + 1);
                return obj;
          }

          // Term ::= Sym
          case TOKEN_SYM:
          {     // Skip over the sigil char when copying the name.
                shivver_parse_shift(state);
                obj_t* obj = aSymA(state->curr_len - 1, state->curr_str + 1);
                return obj;
          }

          // Term ::= Prm
          case TOKEN_PRM:
          {     // Skip over the sigil char and copy the main part of the
                // name into a local buffer so that we can null-terminate it.
                shivver_parse_shift(state);
                size_t nStr     = state->curr_len  - 1;
                char*  str      = (char*)alloca(nStr + 1);
                memcpy(str, state->curr_str + 1, nStr);
                str[nStr] = 0;

                // Use the prim parsing functions to see if the prim name
                // is one of the predefine dones.
                obj_t* obj = 0;

                obj     = shivver_parse_nat_lit(str);
                if (obj != 0) return obj;

                obj     = shivver_parse_nat_prim(str);
                if (obj != 0) return obj;

                // If the previous prim parsing functions did not recognize
                // the prim name then just stash the name in a raw Prm node.
                obj     = aPrmA(nStr, state->curr_str + 1);
                return obj;
          }

          // Term ::= '{' Name* '}' Term
          case TOKEN_CBRA:
          {     shivver_parse_shift(state);
                objlist_t* list = shivver_parse_varSpaceList(state);
                shivver_parse_tok(state, TOKEN_CKET);
                obj_t* objBody  = shivver_parse_term1(state);
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
                obj_t* oResult  = shivver_parse_term1(state);
                shivver_parse_tok(state, TOKEN_RKET);
                return oResult;
          }

          default:
          {     //  Build an error message in freshly allocated space.
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


// Parse a list of terms separated by commas.
objlist_t*
shivver_parse_termCommaList
        (parser_t* state)
{
        objlist_t* list = shivver_objlist_alloc();

        while (true)
        {       shivver_parse_peek(state);
                if (! shivver_parse_isTermStart(state->peek_tok))
                        return list;

                obj_t* obj = shivver_parse_term1(state);
                shivver_objlist_append(list, obj);

                shivver_parse_peek(state);
                if (state->peek_tok == TOKEN_COMMA)
                {       shivver_parse_shift(state);
                }
                else break;
        }

        return list;
}


// Parse a list of variable names separated by whitespace.
objlist_t*
shivver_parse_varSpaceList
        (parser_t* state)
{
        objlist_t* list = shivver_objlist_alloc();

        while(true)
        {       shivver_parse_peek(state);
                if (! (state->peek_tok == TOKEN_VAR))
                        return list;

                shivver_parse_shift(state);
                obj_t* obj = aVarA(state->curr_len, state->curr_str);
                shivver_objlist_append(list, obj);
        }
}


// Check if the given token can start a term.
bool
shivver_parse_isTermStart
        (size_t tok)
{
        switch(tok)
        { case TOKEN_RBRA:
          case TOKEN_SBRA:
          case TOKEN_CBRA:
          case TOKEN_VAR:
          case TOKEN_SYM:
          case TOKEN_PRM:
          case TOKEN_MAC:
                return true;
        }
        return false;
}
