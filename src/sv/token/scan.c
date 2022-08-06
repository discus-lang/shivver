
#include "sv/token.h"
#include "sv/token/state.h"

// Scan the next token from the state.
bool    sv_token_scan
        ( sv_token_state_t*        state
        , sv_token_t*              out_token)
{
 // Try to scan a token from the current state.
 // We jump back here to bypass whitespace.
 again:
        // Input ends as we are passed the specified length,
        //  or we have found a null string terminating character.
        if (state->next >= state->length
         || state->input[state->next] == 0) {
                out_token->meta.tag             = sv_token_meta_end;
                out_token->meta.range.first     = state->pos;
                out_token->meta.range.final     = state->pos;
                return true;
        }

        // The first character determines what sort of token this is.
        switch(state->input[state->next]) {
         // end of string
         case ' ':
                state->next++;
                state->pos.column++;
                goto again;

         // tab character
         case '\t':
                state->next++;
                state->pos.column += state->tab_columns;
                goto again;

         // newline character
         case '\n':
                state->next++;
                state->pos.column = 1;
                state->pos.line++;
                goto again;

         // punctuation
         case '(': out_token->atom.tag = sv_token_atom_rbra;    goto single;
         case ')': out_token->atom.tag = sv_token_atom_rket;    goto single;
         case '{': out_token->atom.tag = sv_token_atom_cbra;    goto single;
         case '}': out_token->atom.tag = sv_token_atom_cket;    goto single;
         case '[': out_token->atom.tag = sv_token_atom_sbra;    goto single;
         case ']': out_token->atom.tag = sv_token_atom_sket;    goto single;
         case ',': out_token->atom.tag = sv_token_atom_comma;   goto single;
         case ';': out_token->atom.tag = sv_token_atom_semi;    goto single;
         case '=': out_token->atom.tag = sv_token_atom_eq;      goto single;

        }

        assert(false);

 // Produce a single character token
 single:
        state->next++;
        state->pos.column++;
        return true;
}