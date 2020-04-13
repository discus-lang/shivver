
#include <assert.h>
#include <stdarg.h>
#include "shivver/eval.h"
#include "shivver/eval/error.h"
#include "shivver/eval/term/base.h"
#include "shivver/eval/term/apph.h"
#include "shivver/eval/term/apsh.h"
#include "shivver/prim.h"
#include "shivver/util.h"


// Force evaluate a term, expecting a single result value.
//  On success, the result is returned directly.
//  On failure, we return 0 and the error message is in state->error_str.
//   The messsage string in state->error_str needs to be freed by the caller.
obj_t*
shivver_eval_term1_null
        ( eval_t*       state   // Evaluation state.
        , obj_t*        oEnv    // Evaluation environemnt.
        , obj_t*        oExp)   // Term expression to evaluate.
{
        int ret = setjmp(state->jmp_err);
        if (ret == 0) {
                obj_t* oRes = 0;
                shivver_eval_termN (state, 1, &oRes, oEnv, oExp);
                return oRes;
        }
        else {
                // We got longjmped to, so there was a parse error.
                // An error message need to have been set in the state.
                assert(state->error_str != 0);

                // Signal to the caller that there was an error by
                // returning 0.
                return 0;
        }
}


// Like 'shivver_eval_termN', but we also unfold macros names in head position.
void
shivver_force_termN
        ( eval_t*       state   // Evaluation state.
        , size_t        nArity  // Number of values we expect from evaluation.
        , obj_t**       osRes   // Array of pointers to fill with results.
        , obj_t*        oEnv    // Evaluation environment.
        , obj_t*        oExp)   // Term expression to evaluate.
{ again:
  switch(xObj_tag(oExp))
  { case TAG_MACA:
    {   reqeval ( state, nArity == 1
                , "Arity mismatch for macro -- need '%d', have '1'."
                , nArity);

        obj_t* oRes
         = shivver_eval_resolve_mac
                (state->decls, xMacA_name(oExp));

        reqeval ( state, oRes != 0
                , "Macro '%s' is undefined."
                , xMacA_name(oExp));
        oExp = oRes;
        goto again;
    }
  }
  shivver_eval_termN (state, nArity, osRes, oEnv, oExp);
}


// Evaluate a term, expecting the given number of result values.
void
shivver_eval_termN
        ( eval_t*       state   // Evaluation state.
        , size_t        nArity  // Number of values we expect from evaluation.
        , obj_t**       osRes   // Array of pointers to fill with results.
        , obj_t*        oEnv    // Evaluation environment.
        , obj_t*        oExp)   // Term expression to evaluate.
{ again:
  switch(xObj_tag(oExp))
  { // atomic -------------------------------------------------
    case TAG_SYMA:
    case TAG_PRMA:
    case TAG_PRZA:
    case TAG_NATA:
    case TAG_MACA:
    case TAG_SYMT:
    case TAG_PRMT:
        shivver_eval_termN_Atom(state, nArity, osRes, oEnv, oExp);
        return;

    case TAG_VARA:
        shivver_eval_termN_VarA(state, nArity, osRes, oEnv, oExp);
        return;


    // hot ----------------------------------------------------
    case TAG_MMMH:
        shivver_eval_termN_MmmH(state, nArity, osRes, oEnv, oExp);
        return;

    case TAG_ABSH:
        shivver_eval_termN_AbsH(state, nArity, osRes, oEnv, oExp);
        return;

    case TAG_APPH:
        if (shivver_eval_termN_AppH(state, nArity, osRes, &oEnv, &oExp))
                goto again;
        return;

    case TAG_APSH:
        if (shivver_eval_termN_ApsH(state, nArity, osRes, &oEnv, &oExp))
                goto again;
        return;


    // static  --------------------------------------------------
    case TAG_VART:
        shivver_eval_termN_VarT(state, nArity, osRes, oEnv, oExp);
        return;

    // Some object with an unrecognized tag,
    //  or maybe just trash in the heap.
    default:
        reqeval ( state, false
                , "Cannot evaluate object.");
  }
}


