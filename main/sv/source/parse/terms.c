
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "sv/source.h"
#include "sv/store.h"


// Get the length of a terms list.
size_t
sv_source_term_list_length(
        sv_source_term_list_t* terms)
{
        size_t count = 0;
        for(sv_source_term_list_t* link = terms;
            link != 0; link = link->tail) {
            count++;
        }

        return count;
}


// Pack all terms in a list into the given array.
void
sv_source_term_list_pack(
        sv_source_term_list_t* terms,
        sv_source_term_t** term)
{
        size_t index = 0;
        for(sv_source_term_list_t* link = terms;
            link != 0; link = link->tail) {
            term[index] = link->head;
            index++;
        }
}


// Parse a possibly empty list of comma separated terms.
//
// Terms ::= . Term CommaTerms
//        |  . e
//
sv_source_term_list_t*
sv_source_parse_terms_comma(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        if (sv_source_parse_term_start(state)) {
                sv_source_term_t* term
                 = sv_source_parse_term(region, state);

                sv_source_term_list_t* rest
                 = sv_source_parse_terms_comma1(region, state);

                sv_source_term_list_t* terms
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_list_t));

                terms->head = term;
                terms->tail = rest;
                return terms;
        }

        return 0;
}


// Parse a non-empty sequence of comma terms.
//
// CommaTerms ::= . ',' Term CommaTerms
//             |  . e
//
sv_source_term_list_t*
sv_source_parse_terms_comma1(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        if (state->here.super.tag == sv_token_atom_comma)
        {
                sv_source_parse_shift(state);

                sv_source_term_t* term
                 = sv_source_parse_term(region, state);

                sv_source_term_list_t* rest
                 = sv_source_parse_terms_comma1(region, state);

                sv_source_term_list_t* terms
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_list_t));

                terms->head = term;
                terms->tail = rest;
                return terms;
        }

        return 0;
}

