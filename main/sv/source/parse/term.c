
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "sv/source.h"
#include "sv/store.h"


// Accept a term from the parser state,
//  allocating the nodes into the given region.
sv_source_term_t*
sv_source_parse_term(
        sv_source_parse_t* state,
        sv_store_region_t* region)
{
        sv_source_term_t* term
         = sv_source_parse_term_atom(
                state, region);

        return term;
}


// Accept an atomic term from the parser state,
//  allocating the nodes into the given region.
sv_source_term_t*
sv_source_parse_term_atom(
        sv_source_parse_t* state,
        sv_store_region_t* region)
{
        switch(state->here.super.tag) {
         case sv_token_name_var:
         case sv_token_name_sym:
         case sv_token_name_prm:
         case sv_token_name_mac:
         case sv_token_name_nom:
         {
                char*  first = state->here.name.first;
                size_t count = state->here.name.count;

                // Allocate term node including space for the name.
                sv_source_term_name_t* mName
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_name_t) + count + 1);

                // Token range.
                mName->range = state->here.name.range;

                // Set term tag based on the token sort.
                switch(state->here.name.tag) {
                 case sv_token_name_var: mName->tag = sv_source_name_var; break;
                 case sv_token_name_sym: mName->tag = sv_source_name_sym; break;
                 case sv_token_name_prm: mName->tag = sv_source_name_prm; break;
                 case sv_token_name_mac: mName->tag = sv_source_name_mac; break;
                 case sv_token_name_nom: mName->tag = sv_source_name_nom; break;
                 default: assert(false);
                }

                // Copy name text into the node, including null terminator.
                memcpy(mName->name, first, count);
                mName->name[count] = 0;
                mName->length = count;

                return (sv_source_term_t*)mName;
         }

         default:
                assert(false);
        }

        assert(false);
}