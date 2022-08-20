
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
         // Term ::= Var | Def | Nom | Sym | Prm
         case sv_token_name_var:
         case sv_token_name_def:
         case sv_token_name_nom:
         case sv_token_name_sym:
         case sv_token_name_prm:

         // Term ::= . '[' Term, ']'
         case sv_token_atom_sbra:

         // Term ::= . '{' Binder* '}' Term
         case sv_token_atom_cbra:

         // Term ::= . '!let' '{' Binder* '}' '=' Term '!in' Term
         case sv_token_atom_let:

         // Term ::= . '!rec' Var '=' Term ('!and' ...)* '!in' Term
         case sv_token_atom_rec:

         // Term ::= . '!box' Term
         case sv_token_atom_box:

         // Term ::= . '!run' Term
         case sv_token_atom_run:

         // Term ::= . '(' Term ')'
         case sv_token_atom_rbra:
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
        assert(region != 0);
        assert(state  != 0);

        // Parse a term list.
        sv_source_term_tree_t* terms
         = sv_source_parse_terms1(region, state);
        assert(terms != 0);

        // Count the number of terms in the whole argument list.
        size_t nTerms
         = sv_source_term_tree_size(terms);
        assert(nTerms >= 1);

        // Flatten the term tree into a flat array.
        sv_source_term_t* array[nTerms];
        sv_source_term_tree_pack(terms, array, 0);

        // If there was only one term in the list then return it directly,
        // otherwise construct a chain of application nodes.
        return sv_source_parse_term_build_app(
                region, array[0], array, nTerms, 1);
}


// Accept an atomic term from the parser state,
//  allocating the nodes into the given region.
sv_source_term_t*
sv_source_parse_term_base(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        assert(region != 0);
        assert(state  != 0);

        switch(state->here.super.tag) {

        // Term ::= . Var | . Def | . Nom | . Sym | . Prm
        case sv_token_name_var:
        case sv_token_name_def:
        case sv_token_name_nom:
        case sv_token_name_sym:
        case sv_token_name_prm:
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
                 case sv_token_name_def: mName->tag = sv_source_term_def; break;
                 case sv_token_name_nom: mName->tag = sv_source_term_nom; break;
                 case sv_token_name_sym: mName->tag = sv_source_term_sym; break;
                 case sv_token_name_prm: mName->tag = sv_source_term_prm; break;
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

        // Term ::= . '[' Term,* ']' (& '[' Term,* ']')*
        case sv_token_atom_sbra:
        {
                sv_token_pos_t posFirst
                 = state->here.super.range.first;
                sv_source_parse_shift(state);

                sv_source_term_tree_t* msArg
                 = sv_source_parse_terms_comma(region, state);

                sv_token_pos_t posFinal
                 = state->here.super.range.final;
                sv_source_parse_token(state, sv_token_atom_sket);

                // Term ::= '[' Term,* ']' . (& Term)*
                while (state->here.name.tag == sv_token_atom_ampersand) {
                        sv_source_parse_shift(state);

                        sv_source_term_t* term
                         = sv_source_parse_term(region, state);
                        assert(term != 0);

                        sv_source_term_tree_leaf_t* msLeaf
                         = sv_store_region_alloc(region,
                                sizeof(sv_source_term_tree_leaf_t));
                        msLeaf->tag  = 0;
                        msLeaf->term = term;

                        sv_source_term_tree_join_t* msJoin
                         = sv_store_region_alloc(region,
                                sizeof(sv_source_term_tree_join_t));
                        msJoin->tag   = 2;
                        msJoin->left  = msArg;
                        msJoin->right = (sv_source_term_tree_t*)msLeaf;
                        msArg = (sv_source_term_tree_t*)msJoin;
                }

                sv_source_term_mmm_t* mMmm
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_mmm_t));

                mMmm->range.first = posFirst;
                mMmm->range.final = posFinal;
                mMmm->tag         = sv_source_term_mmm;
                mMmm->args        = msArg;

                return (sv_source_term_t*)mMmm;
        }

        // Term ::= . '{' Binder* '}' Term
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

        // Term ::= . '!let' '{' Binder* '}' '=' Term '!in' Term
        //       |  . '!let' Binder '=' Term '!in' Term
        case sv_token_atom_let:
        {
                sv_token_pos_t posFirst
                 = state->here.super.range.first;
                sv_source_parse_shift(state);

                sv_source_binders_t* binders = 0;
                if (state->here.super.tag == sv_token_atom_cbra) {
                        sv_source_parse_shift(state);
                        binders = sv_source_parse_binders(region, state);
                        sv_source_parse_token(state, sv_token_atom_cket);
                }
                else {
                        binders = sv_source_parse_binders(region, state);
                        assert(binders->next == 0);
                }

                sv_source_parse_token(state, sv_token_atom_equals);

                sv_source_term_t* mBound
                 = sv_source_parse_term(region, state);

                sv_source_parse_token(state, sv_token_atom_in);

                sv_source_term_t* mBody
                 = sv_source_parse_term(region, state);
                sv_token_pos_t posFinal
                 = mBody->super.range.final;

                sv_source_term_let_t* mLet
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_let_t));

                mLet->range.first = posFirst;
                mLet->range.final = posFinal;
                mLet->tag         = sv_source_term_let;
                mLet->binders     = binders;
                mLet->bound       = mBound;
                mLet->body        = mBody;

                return (sv_source_term_t*)mLet;
        }

        // Term ::= . '!rec' Var '=' Term (!and ...) '!in' Term
        case sv_token_atom_rec:
        {
                sv_token_pos_t posFirst
                 = state->here.super.range.first;
                sv_source_parse_shift(state);

                sv_source_bindings_t* bindings
                 = sv_source_parse_bindings(region, state);

                sv_source_parse_token(state, sv_token_atom_in);

                sv_source_term_t* mBody
                 = sv_source_parse_term(region, state);
                sv_token_pos_t posFinal
                 = mBody->super.range.final;

                sv_source_term_rec_t* mRec
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_rec_t));

                mRec->range.first = posFirst;
                mRec->range.final = posFinal;
                mRec->tag         = sv_source_term_rec;
                mRec->bindings    = bindings;
                mRec->body        = mBody;

                return (sv_source_term_t*)mRec;
        }

        // Term ::= . '!box' Term
        case sv_token_atom_box:
        {
                sv_token_pos_t posFirst
                 = state->here.super.range.first;

                sv_source_parse_shift(state);
                sv_source_term_t* mBody
                 = sv_source_parse_term(region, state);

                sv_source_term_box_t* mBox
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_box_t));

                mBox->range.first = posFirst;
                mBox->range.final = mBody->super.range.final;
                mBox->tag         = sv_source_term_box;
                mBox->body        = mBody;

                return (sv_source_term_t*)mBox;
        }

        // Term ::= . '!run' Term
        case sv_token_atom_run:
        {
                sv_token_pos_t posFirst
                 = state->here.super.range.first;

                sv_source_parse_shift(state);
                sv_source_term_t* mBody
                 = sv_source_parse_term(region, state);

                sv_source_term_box_t* mRun
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_term_box_t));

                mRun->range.first = posFirst;
                mRun->range.final = mBody->super.range.final;
                mRun->tag         = sv_source_term_run;
                mRun->body        = mBody;

                return (sv_source_term_t*)mRun;
        }

        // Term ::= . '(' Term ')'
        case sv_token_atom_rbra:
        {
                sv_source_parse_shift(state);
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


// Construct a left biasesd chain of application notes from a list
// of component terms.
//
// For example we convert terms a b c d to applications (((a b) c) d).
//
sv_source_term_t*
sv_source_parse_term_build_app(
        sv_store_region_t*      region,
        sv_source_term_t*       mFun,
        sv_source_term_t**      msArgs,
        size_t                  nArgs,
        size_t                  iArgs)
{
        assert(region != 0);
        assert(mFun   != 0);
        assert(msArgs != 0);

        // No more arguments, return functional term.
        if(iArgs >= nArgs) {
                return mFun;
        }

        // Construct new application node for the functional term,
        // and its first argument.
        sv_source_term_app_t* mApp
         = sv_store_region_alloc(region,
                sizeof(sv_source_term_app_t));

        sv_source_term_t* mArg
         = msArgs[iArgs];

        mApp->range.first = mFun->super.range.first;
        mApp->range.final = mArg->super.range.final;
        mApp->tag = sv_source_term_app;
        mApp->fun = mFun;
        mApp->arg = mArg;

        // Recursively construct applications for the rest of the arguments.
        return sv_source_parse_term_build_app(
                region,
                (sv_source_term_t*)mApp,
                msArgs, nArgs, iArgs + 1);
}
