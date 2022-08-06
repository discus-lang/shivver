
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

         // symbol name
         case '%':
         case '#':
         case '@':
         case '*':
                char* first = state->input + state->next;
                switch(*first) {
                 case '%': out_token->name.tag = sv_token_name_sym; break;
                 case '#': out_token->name.tag = sv_token_name_prm; break;
                 case '@': out_token->name.tag = sv_token_name_mac; break;
                 case '*': out_token->name.tag = sv_token_name_nom; break;
                 default: assert(false);
                }

                size_t remain = state->length - state->next;
                size_t length = sv_token_scan_sigil_name(first, remain);

                out_token->name.range.first = state->pos;
                state->pos.column += length;
                out_token->name.range.final = state->pos;

                out_token->name.first = first;
                out_token->name.count = length;
                state->next += length;
                return true;
        }

        assert(false);

 // Produce a single character token
 single:
        state->next++;
        state->pos.column++;
        return true;
}


// Scan a name token returning the raw length of the token
// in the input.
size_t  sv_token_scan_sigil_name
        (char* str, size_t strLen)
{
        size_t len = 0;

        // skip across sigil.
        str++; strLen--; len++;

        while(  strLen > 0
         && (   (*str >= 'a' && *str <= 'z')
             || (*str >= 'A' && *str <= 'Z')
             || (*str >= '0' && *str <= '9')
             || (*str == '\'')
             || (*str == '_')))
        {
                str++; strLen--; len++;
        }

        return len;
}
