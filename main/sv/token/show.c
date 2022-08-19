
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sv/token.h"


// Show a token tag as a freshly allocated string.
char*   sv_token_show_tag(
        sv_token_tag_t tag)
{
        switch(tag) {
         case sv_token_meta_none:       return strdup("none");          break;
         case sv_token_meta_end:        return strdup("end");           break;

         case sv_token_atom_def:        return strdup("def");           break;
         case sv_token_atom_let:        return strdup("let");           break;
         case sv_token_atom_rec:        return strdup("rec");           break;
         case sv_token_atom_and:        return strdup("and");           break;
         case sv_token_atom_in:         return strdup("in");            break;
         case sv_token_atom_box:        return strdup("box");           break;
         case sv_token_atom_run:        return strdup("run");           break;

         case sv_token_atom_rbra:       return strdup("rbra");          break;
         case sv_token_atom_rket:       return strdup("rket");          break;
         case sv_token_atom_cbra:       return strdup("cbra");          break;
         case sv_token_atom_cket:       return strdup("cket");          break;
         case sv_token_atom_sbra:       return strdup("sbra");          break;
         case sv_token_atom_sket:       return strdup("sket");          break;
         case sv_token_atom_comma:      return strdup("comma");         break;
         case sv_token_atom_semicolon:  return strdup("semicolon");     break;
         case sv_token_atom_equals:     return strdup("equals");        break;
         case sv_token_atom_ampersand:  return strdup("ampersand");     break;

         case sv_token_name_var:        return strdup("var");           break;
         case sv_token_name_sym:        return strdup("sym");           break;
         case sv_token_name_prm:        return strdup("prm");           break;
         case sv_token_name_mac:        return strdup("mac");           break;
         case sv_token_name_nom:        return strdup("nom");           break;

         case sv_token_lit_nat:         return strdup("nat");           break;

         default: assert(false);
        }
}


// Show a token position as a freshly allocated string.
char*   sv_token_show_pos(
        sv_token_pos_t pos)
{
        char* result = 0;
        asprintf(&result, "%zu:%zu", (size_t)pos.line, (size_t)pos.column);
        return result;
}


// Show a token range as a freshly allocated string.
char*   sv_token_show_range(
        sv_token_range_t range)
{
        char* first = sv_token_show_pos(range.first);
        char* final = sv_token_show_pos(range.final);

        char* result = 0;
        asprintf(&result, "%s-%s", first, final);
        free(first); free(final);
        return result;
}


// Show a token as a freshly allocated string.
char*   sv_token_show(
        sv_token_t token)
{
        char* sRange    = sv_token_show_range(token.super.range);
        char* sTag      = sv_token_show_tag(token.super.tag);

        char* sResult;
        switch(token.super.sort) {
         case sv_token_sort_meta:
                asprintf(&sResult,
                 "{ range = \"%s\", tag = %%%s }", sRange, sTag);
                break;

         case sv_token_sort_atom:
                asprintf(&sResult,
                 "{ range = \"%s\", tag = %%%s }", sRange, sTag);
                break;

         case sv_token_sort_name:
         {
                char sName[token.name.count + 1];
                memcpy(sName, token.name.first, token.name.count);
                sName[token.name.count] = 0;
                asprintf(&sResult,
                 "{ range = \"%s\", tag = %%%s, name = '%s' }",
                 sRange, sTag, sName);
                break;
         }

         case sv_token_sort_lit:
                asprintf(&sResult,
                 "{ range = \"%s\", tag = %%%s, value = %zu }"
                 , sRange, sTag, token.lit_nat.value);
                break;

         default:
                assert(false);
        }

        free(sRange); free(sTag);
        return sResult;
}
