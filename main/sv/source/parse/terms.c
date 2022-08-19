
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "sv/source.h"
#include "sv/store.h"

// Get the number of leaves in a term tree
size_t
sv_source_term_tree_size(
        sv_source_term_tree_t* terms)
{
        if (terms == 0) return 0;
        switch(terms->super.tag) {
         case 0: return 1;
         case 1: return 1 + sv_source_term_tree_size(terms->cons.tail);
         case 2: return sv_source_term_tree_size(terms->join.left)
                      + sv_source_term_tree_size(terms->join.right);
         default: assert(false);
        }
}


// Pack all terms in a list into the given array.
size_t
sv_source_term_tree_pack(
        sv_source_term_tree_t* terms,
        sv_source_term_t** term,
        size_t offset0)
{
        if (terms == 0) return offset0;
        switch(terms->super.tag) {
         case 0: {
                sv_source_term_t* tt = terms->leaf.term;
                term[offset0] = tt;
                return offset0 + 1;
         }

         case 1: {
                sv_source_term_t* tt = terms->cons.head;
                term[offset0] = tt;
                size_t offset2
                 = sv_source_term_tree_pack(
                        terms->cons.tail, term, offset0 + 1);
                return offset2;
         }

         case 2: {
                size_t offset1
                 = sv_source_term_tree_pack(
                        terms->join.left, term, offset0);
                size_t offset2
                 = sv_source_term_tree_pack(
                        terms->join.right, term, offset1);
                return offset2;
         }

         default: assert(false);
        }
}


// Parse a possibly empty sequence of terms.
sv_source_term_tree_t*
sv_source_parse_terms(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        if (sv_source_parse_term_start(state)) {
                return sv_source_parse_terms1(region, state);
        }

        return 0;
}


// Parse a non-empty sequence of terms.
sv_source_term_tree_t*
sv_source_parse_terms1(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        sv_source_term_t* term
         = sv_source_parse_term_base(region, state);

        sv_source_term_tree_cons_t* terms
         = sv_store_region_alloc(region,
                sizeof(sv_source_term_tree_cons_t));
        terms->tag  = 1;
        terms->head = term;

        if (sv_source_parse_term_start(state)) {
                sv_source_term_tree_t* rest
                 = sv_source_parse_terms1(region, state);

                terms->tail = rest;
                return (sv_source_term_tree_t*)terms;
        }
        else {
                terms->tail = 0;
                return (sv_source_term_tree_t*)terms;
        }
}


// Parse a possibly empty list of comma separated terms.
//
// Terms ::= . Term CommaTerms
//        |  . e
//
sv_source_term_tree_t*
sv_source_parse_terms_comma(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        if (sv_source_parse_term_start(state)) {
                sv_source_term_t* term
                 = sv_source_parse_term(region, state);

                sv_source_term_tree_t* rest
                 = sv_source_parse_terms_comma1(region, state);

                sv_source_term_tree_cons_t* terms
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_tree_cons_t));

                terms->tag  = 1;
                terms->head = term;
                terms->tail = rest;
                return (sv_source_term_tree_t*)terms;
        }

        return 0;
}


// Parse a non-empty sequence of comma terms.
//
// CommaTerms ::= . ',' Term CommaTerms
//             |  . e
//
sv_source_term_tree_t*
sv_source_parse_terms_comma1(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        if (state->here.super.tag == sv_token_atom_comma) {
                sv_source_parse_shift(state);

                sv_source_term_t* term
                 = sv_source_parse_term(region, state);

                sv_source_term_tree_t* rest
                 = sv_source_parse_terms_comma1(region, state);

                sv_source_term_tree_cons_t* terms
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_tree_cons_t));

                terms->tag  = 1;
                terms->head = term;
                terms->tail = rest;
                return (sv_source_term_tree_t*)terms;
        }

        return 0;
}

