
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "sv/source.h"
#include "sv/store.h"


// Accept a term from the parser state,
//  allocating the nodes into the given region.
sv_source_term_t*
sv_source_parse_term(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        sv_source_term_t* term
         = sv_source_parse_term0(
                region, state);

        return term;
}


// Accept an atomic term from the parser state,
//  allocating the nodes into the given region.
sv_source_term_t*
sv_source_parse_term0(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        switch(state->here.super.tag) {

        // Term ::= Var | Sym | Prm | Mac | Nom
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
                 case sv_token_name_var: mName->tag = sv_source_term_var; break;
                 case sv_token_name_sym: mName->tag = sv_source_term_sym; break;
                 case sv_token_name_prm: mName->tag = sv_source_term_prm; break;
                 case sv_token_name_mac: mName->tag = sv_source_term_mac; break;
                 case sv_token_name_nom: mName->tag = sv_source_term_nom; break;
                 default: assert(false);
                }

                // Copy name text into the node, including null terminator.
                memcpy(mName->name, first, count);
                mName->name[count] = 0;
                mName->length = count;

                // Accept the token.
                sv_source_parse_shift(state);

                return (sv_source_term_t*)mName;
         }

//        case sv_token_atom_sbra:
//        {
//                sv_token_pos_t posFirst
//                 = state->here.super.range.first;
//                sv_source_parse_shift(state);
//
//        }

        // Term ::= '{' Name* '}' Term
        case sv_token_atom_cbra:
        {
                sv_token_pos_t posFirst
                 = state->here.super.range.first;
                sv_source_parse_shift(state);

                sv_source_binders_t* binders
                 = sv_source_parse_binders(region, state);

                sv_source_parse_token(state, sv_token_atom_cket);

                sv_source_term_t* mBody
                 = sv_source_parse_term(region, state);
                sv_token_pos_t posFinal
                 = mBody->super.range.final;

                sv_source_term_abs_t* mAbs
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_abs_t));

                mAbs->range.first = posFirst;
                mAbs->range.final = posFinal;
                mAbs->tag         = sv_source_term_abs;
                mAbs->binders     = binders;
                mAbs->body        = mBody;

                return (sv_source_term_t*)mAbs;
        }

        // Term ::= '(' Term ')'
        case sv_token_atom_rbra:
        {       sv_source_parse_shift(state);
                sv_source_term_t* term
                 = sv_source_parse_term(region, state);
                sv_source_parse_token(state, sv_token_atom_rket);
                return term;
        }

        default:
                assert(false);
        }

        assert(false);
}

