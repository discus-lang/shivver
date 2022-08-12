
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "sv/source.h"
#include "sv/store.h"


// Check if the next token on the input starts a term.
bool
sv_source_parse_term_start(
        sv_source_parse_t* state)
{
        switch(state->here.super.tag) {
         // Term ::= Var | Sym | Prm | Mac | Nom
         case sv_token_name_var:
         case sv_token_name_sym:
         case sv_token_name_prm:
         case sv_token_name_mac:
         case sv_token_name_nom:

         // Term ::= . '[' Term, ']'
         case sv_token_atom_sbra:

         // Term ::= . '{' (Demand Var)* '}' Term
         case sv_token_atom_cbra:

         // Term ::= . '!let' '{' (Demand Var)* '}' '=' Term '!in' Term
         case sv_token_atom_let:

         // Term ::= . '!rec' Var '=' Term ('!and' ...)* '!in' Term
         case sv_token_atom_rec:

         // Term ::= . '!box' Term
         case sv_token_atom_box:

         // Term ::= . '!run' Term
         case sv_token_atom_run:
                return true;

         default:
                return false;
        }
}


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

        // Term ::= '[' Term,* ']'
        case sv_token_atom_sbra:
        {
                sv_token_pos_t posFirst
                 = state->here.super.range.first;
                sv_source_parse_shift(state);

                sv_source_term_list_t* terms
                 = sv_source_parse_terms_comma(region, state);

                sv_token_pos_t posFinal
                 = state->here.super.range.final;
                sv_source_parse_token(state, sv_token_atom_sket);

                size_t count
                 = sv_source_term_list_length(terms);

                sv_source_term_mmm_t* mMmm
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_mmm_t) + sizeof(sv_source_term_t*) * count);

                mMmm->range.first = posFirst;
                mMmm->range.final = posFinal;
                mMmm->tag   = sv_source_term_mmm;
                mMmm->count = count;
                sv_source_term_list_pack(terms, (sv_source_term_t**)&(mMmm->arg));

                return (sv_source_term_t*)mMmm;
        }

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


