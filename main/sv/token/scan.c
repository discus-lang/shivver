// Token scanner for the core language.
//
//  The syntax of the core language is designed so it is very easy to tokenize.
//  The form of each token can be determined from the first character,
//  and there is no need to backtrack when accepting each token.
//
//  Each token is tagged with the position of the starting and ending characters.
//
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "sv/token.h"

// Scan the next token from the state.
bool    sv_token_scan(
        sv_token_state_t*    state,
        sv_token_t*          out_token)
{
        // Pointer to first character to consider.
        char*  first;

        // Remaining characters in the input string.
        size_t remain;

 // Try to scan a token from the current state.
 // We jump back here to bypass whitespace.
 again:
        first   = state->input + state->next;
        remain  = state->length - state->next;

        // Input ends as we are passed the specified length,
        //  or we have found a null string terminating character.
        if (state->next >= state->length
         || *first == 0) {
                out_token->meta.sort            = sv_token_sort_meta;
                out_token->meta.tag             = sv_token_meta_end;
                out_token->meta.range.first     = state->pos;
                out_token->meta.range.final     = state->pos;
                return true;
        }

        // The first character determines what sort of token this is.
        switch(*first) {
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
         case '~': out_token->atom.tag = sv_token_atom_tilde;   goto single;
         case ';': out_token->atom.tag = sv_token_atom_semi;    goto single;
         case '=': out_token->atom.tag = sv_token_atom_eq;      goto single;

         // symbol name
         case '%':
         case '#':
         case '@':
         case '?':
         {
                out_token->meta.sort = sv_token_sort_name;

                switch(*first) {
                 case '%': out_token->name.tag = sv_token_name_sym; break;
                 case '#': out_token->name.tag = sv_token_name_prm; break;
                 case '@': out_token->name.tag = sv_token_name_mac; break;
                 case '?': out_token->name.tag = sv_token_name_nom; break;
                 default: assert(false);
                }

                size_t length = sv_token_scan_sigil_name(first, remain);

                // We only accepted the sigil and not the next character.
                if(length == 1)
                        return false;

                out_token->name.range.first = state->pos;
                out_token->name.range.final = state->pos;
                out_token->name.range.final.column += length - 1;

                // Skip over the sigil when recording the token name.
                out_token->name.first = first + 1;
                out_token->name.count = length - 1;

                state->pos.column += length;
                state->next += length;
                return true;
         }

         case '!':
         {
                out_token->meta.sort = sv_token_sort_atom;

                size_t keyLength = 0;

                if      (sv_token_matches_keyword("!def", first, remain, &keyLength) > 0) {
                        out_token->name.tag = sv_token_atom_def;
                }
                else if (sv_token_matches_keyword("!let", first, remain, &keyLength) > 0) {
                        out_token->name.tag = sv_token_atom_let;
                }
                else if (sv_token_matches_keyword("!rec", first, remain, &keyLength) > 0) {
                        out_token->name.tag = sv_token_atom_rec;
                }
                else if (sv_token_matches_keyword("!and", first, remain, &keyLength) > 0) {
                        out_token->name.tag = sv_token_atom_and;
                }
                else if (sv_token_matches_keyword("!in", first,  remain, &keyLength) > 0) {
                        out_token->name.tag = sv_token_atom_in;
                }
                else if (sv_token_matches_keyword("!box", first,  remain, &keyLength) > 0) {
                        out_token->name.tag = sv_token_atom_box;
                }
                else if (sv_token_matches_keyword("!run", first,  remain, &keyLength) > 0) {
                        out_token->name.tag = sv_token_atom_run;
                }
                else return false;

                out_token->atom.range.first = state->pos;
                out_token->atom.range.final = state->pos;
                out_token->atom.range.final.column += keyLength - 1;

                state->pos.column += keyLength;
                state->next += keyLength;
                return true;
         }

         default:
                // variable names
                if(*first >= 'a' && *first <= 'z') {
                        size_t length = sv_token_scan_var(first, remain);
                        assert(length >= 1);

                        out_token->name.range.first = state->pos;
                        out_token->name.range.final = state->pos;
                        out_token->name.range.final.column += length - 1;

                        out_token->name.sort  = sv_token_sort_name;
                        out_token->name.tag   = sv_token_name_var;
                        out_token->name.first = first;
                        out_token->name.count = length;

                        state->pos.column += length;
                        state->next += length;
                        return true;
                }

                // literal natural number
                else if(*first >= '0' && *first <= '9') {
                        size_t nNat   = 0;
                        size_t length = sv_token_scan_lit_nat(first, remain, &nNat);
                        assert(length >= 1);

                        out_token->name.range.first = state->pos;
                        out_token->name.range.final = state->pos;
                        out_token->name.range.final.column += length - 1;

                        out_token->lit_nat.sort  = sv_token_sort_lit;
                        out_token->lit_nat.tag   = sv_token_lit_nat;
                        out_token->lit_nat.value = nNat;

                        state->pos.column += length;
                        state->next += length;
                        return true;
                }
                else return false;
        }

        return false;

 // Produce a single character token
 single:
        out_token->atom.sort = sv_token_sort_atom;
        out_token->atom.range.first = state->pos;
        out_token->atom.range.final = state->pos;
        state->next++;
        return true;
}


// Scan a name token,
//  returning the raw length of the token in the input,
//  including the sigil at the start.
size_t  sv_token_scan_sigil_name(
        char* str, size_t strLen)
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


// Scan a variable name,
//  returning the raw length in the lex buffer.
size_t  sv_token_scan_var(
        char* str, size_t strLen)
{
        size_t len = 0;

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


// Check if the start of the input string matches the given keyword.
//  The specified keyword includes the leading '!' sigil.
bool    sv_token_matches_keyword(
        char* keyword, char* str, size_t strLen,
        size_t* out_keywordLength)
{
        size_t count = 0;

        while( strLen > 0
            && *keyword != '\0'
            && *str == *keyword)
        {
                str++;
                strLen--;
                count++;
                keyword++;
        }

        if (*keyword == 0) {
                *out_keywordLength = count;
                return true;
        }
        else {
                return false;
        }
}


// Scan a natural number.
size_t  sv_token_scan_lit_nat(
        char* first, size_t nRemain,
        size_t* out_value)
{
        char*  str = first;
        size_t nCount = 0;

        while( nRemain > 0
         &&   (*str >= '0' && *str <= '9'))
        {
                str++; nRemain--; nCount++;
        }

        char buf[nCount + 1];
        memcpy(buf, first, nCount);
        buf[nCount] = 0;
        *out_value = atoi(buf);

        return nCount;
}
