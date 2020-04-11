
#include <assert.h>
#include <stdarg.h>
#include "shivver/eval.h"
#include "shivver/prim.h"
#include "shivver/util.h"


// Assert a property is true during evaluation.
//  This is inlined so we get a fast-path that does not need a function-call
//  when the property is true.
static inline void
reqeval ( eval_t*       state
        , bool          prop
        , char*         format
        , ...)
{
        if ( __builtin_expect (prop, true)) return;
        va_list args;
        va_start(args, format);
        shivver_eval_error(state, format, args);
        va_end(args);
}


// Evaluate a term, expecting a single result value.
//  On success, the result is returned directly.
//  On failure, we return 0 and the error message is in state->error_str.
//   The messsage string in state->error_str needs to be freed by the caller.
obj_t*
shivver_eval_term_zero
        ( eval_t*       state   // Evaluation state.
        , obj_t*        oEnv    // Evaluation environemnt.
        , obj_t*        oExp)   // Term expression to evaluate.
{
        int ret = setjmp(state->jmp_err);
        if (ret == 0)
                return shivver_eval_term1 (state, oEnv, oExp);
        else {
                // We got longjmped to, so there was a parse error.
                // An error message need to have been set in the state.
                assert(state->error_str != 0);

                // Signal to the caller that there was an error by
                // returning 0.
                return 0;
        }
}


// Evaluate a term, expecting a single result value.
//  On failure we longjmp to the destination defined in the state.
obj_t*
shivver_eval_term1
        ( eval_t*       state   // Evaluation state.
        , obj_t*        oEnv    // Evaluation environment.
        , obj_t*        oExp)   // Term expression to evaluate.
{
        obj_t*  oRes     = 0;
        shivver_eval_termN(state, 1, &oRes, oEnv, oExp);
        return oRes;
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
    default:
        shivver_eval_termN (state, nArity, osRes, oEnv, oExp);
  }
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
    {   reqeval ( state, nArity == 1
                , "Arity mismatch for atom -- need '%d', have '1'."
                , nArity);
        osRes[0] = oExp;
        return;
    }

    case TAG_VARA:
    {   reqeval ( state, nArity == 1
                , "Arity mismatch for variable -- need '%d', have '1'."
                , nArity);

        obj_t* oRes
         = shivver_eval_resolve_var
                (oEnv, xVarA_name(oExp), xVarA_bump(oExp));

        reqeval ( state, oRes != 0
                , "Variable '%s' is not in scope."
                , xVarA_name(oExp));
        osRes[0] = oRes;
        return;
    }

    case TAG_MACA:
    {     reqeval ( state, nArity == 1
                  , "Arity mismatch for macro -- need '%d', have '1'."
                  , nArity);
          osRes[0] = oExp;
          return;
    }

    // hot ----------------------------------------------------
    case TAG_MMMH:
    {   size_t nLen = xMmmH_len(oExp);
        reqeval ( state, nLen == nArity
                , "Arity mismatch for term vector -- need '%d', have '%d'."
                , nArity, nLen);

        for (size_t i = 0; i < nLen; i++)
                shivver_eval_termN
                        ( state, 1, osRes + i
                        , oEnv, xMmmH_arg(oExp, i));
        return;
    }

    case TAG_ABSH:
    {   reqeval ( state, nArity == 1
                , "Arity mismatch for abstraction -- need '%d', have '1'."
                , nArity);

        obj_t* oAbs     = oExp;
        obj_t* oBody    = xAbsH_body(oAbs);
        osRes[0]        = aCloH ( xAbsH_len(oExp)
                                , oEnv, xAbsH_parmp(oExp), oBody);
        return;
    }

    case TAG_APPH:
    {   obj_t* oApp     = oExp;
        obj_t* oHead    = xAppH_fun(oApp);
        obj_t* oArg     = xAppH_arg(oApp);

        // Evaluate the head.
        obj_t* oHeadV   = 0;
        shivver_force_termN
                ( state, 1, &oHeadV
                , oEnv, oHead);

        uint32_t tag    = xObj_tag(oHeadV);
        switch(tag)
        { case TAG_SYMA:
          {     // For applications of symbols,
                // evaluate all the arguments and rebuild the application.
                obj_t*  oArgV;
                shivver_eval_termN (state, 1, &oArgV, oEnv, oArg);
                osRes[0] = aAppH(oHeadV, oArgV);
                return;
          }

          case TAG_PRZA:
          {     uint32_t pTag   = xPrzA_tag(oHeadV);

                // Evaluate all the arguments.
                size_t nParams  = shivver_prim_tag_args(pTag);
                obj_t* osArgs[nParams];
                shivver_eval_termN (state, nParams, osArgs, oEnv, oArg);

                // Get the number of result values produced by this primitive.
                size_t nResults = shivver_prim_tag_results(pTag);

                // The number of produced results must match that required
                // by the contetxt.
                reqeval ( state, nArity == nResults
                        , "Arity mismatch in prim result -- need '%d', have '%d'."
                        , nArity, nResults);

                // Evaluate the primitive itself.
                shivver_eval_prim (nArity, osRes, pTag, osArgs);
                return;
          }

          case TAG_CLOH:
          {     // For applications of closures,
                // evaluate the arguments, add them to environment and continue
                // with the body expresssion.
                obj_t*  oClo    = oHeadV;
                size_t  nParams = xCloH_len(oClo);

                // Evaluate the arguments.
                obj_t* osArgs[nParams];
                shivver_eval_termN (state, nParams, osArgs, oEnv, oArg);

                // Tail call ourselves with the extended environment
                // to evaluate the body.
                oEnv    = aEnvH ( nParams
                                , xCloH_env(oClo)
                                , xCloH_parmp(oClo), osArgs);

                oExp    = xCloH_body(oClo);
                goto again;
          }

          case TAG_PRMA:
          {     // Primitive name that we don't have an eval function for,
                // or hasn't been mapped to a prz identifier.
                reqeval ( state, false
                        , "Unknown primitive '#%s'.", xPrmA_name(oHeadV));
          }

          default:
          {     reqeval ( state, false
                        , "Cannot apply non-functional value.");
          }
        }
    }

    case TAG_APSH:
    {   obj_t* oAps     = oExp;
        uint32_t nArgs  = xApsH_len(oAps);
        obj_t* oHead    = xApsH_fun(oAps);

        // Evaluate the head.
        obj_t* oHeadV   = 0;
        shivver_force_termN (state, 1, &oHeadV, oEnv, oHead);

        switch(xObj_tag(oHeadV))
        { case TAG_SYMA:
          {     // For applications of symbols,
                // evaluate all the arguments and rebuild the application.
                obj_t* osArgVs[nArgs];
                for(size_t i = 0; i < nArgs; i++)
                {       obj_t* oArg = xApsH_arg(oAps, i);
                        shivver_eval_termN (state, 1, osArgVs + i, oEnv, oArg);
                }

                osRes[0] = aApsH(nArgs, oHeadV, osArgVs);
                return;
          }

          case TAG_PRZA:
          {     uint32_t pTag   = xPrzA_tag(oHeadV);

                // TODO: we can allow some prims to take variable numbers of args,
                // which we want for ctors like #list, and control constructs.
                // encode this flexibility in tag as param number of 0xff.
                size_t nParams  = shivver_prim_tag_args(pTag);
                reqeval ( state, nParams == nArgs
                        , "Arity mismatch in prim application -- need '%d', have '%d'"
                        , nParams, nArgs);

                // Evaluate all the arguments.
                obj_t* osArgs[nArgs];
                for(size_t i = 0; i < nArgs; i++)
                {       obj_t* oArg = xApsH_arg(oAps, i);
                        shivver_eval_termN (state, 1, osArgs + i, oEnv, oArg);
                }

                // Apply the primitive.
                size_t nResults = shivver_prim_tag_results(pTag);
                reqeval ( state, nArity == nResults
                        , "Arity mismatch in prin result -- need '%d', have '%d'"
                        , nArity, nResults);
                shivver_eval_prim (nArity, osRes, pTag, osArgs);
                return;
          }

          case TAG_CLOH:
          {     // For applications of closures,
                // evaluate the arguments, add them to environment and continue
                // with the body expresssion.
                obj_t* oClo     = oHeadV;
                size_t nParams  = xCloH_len(oClo);
                reqeval ( state, nParams == nArgs
                        , "Arity mismatch in function application -- need '%d', have '%d'"
                        , nParams, nArgs);

                // Application of closure with no parameters.
                //  so we have ({} Term) []
                if (nParams == 0)
                {       oExp    = xCloH_body(oClo);
                        goto again;
                }
                else
                {       // Evaluate all the arguments.
                        obj_t* osArgs[nArgs];
                        for(size_t i = 0; i < nArgs; i++)
                        {       obj_t* oArg = xApsH_arg(oAps, i);
                                shivver_eval_termN (state, 1, osArgs + i, oEnv, oArg);
                        }

                        // Tail call ourselves with the extended environment
                        // to evaluate the body.
                        oEnv    = aEnvH ( nParams
                                        , xCloH_env(oClo)
                                        , xCloH_parmp(oClo), osArgs);

                        oExp    = xCloH_body(oClo);
                        goto again;
                }
          }

          case TAG_PRMA:
          {     // Primitive name that we don't have an eval function for,
                // or hasn't been mapped to a prz identifier.
                reqeval ( state, false
                        , "Unknown primitive '#%s'.", xPrmA_name(oHeadV));
          }

          default:
          {     shivver_prim_console_printp(oHeadV);
                reqeval ( state, false
                        , "Cannot apply non-functional value.");
          }
        }
    }


    // static  --------------------------------------------------
    case TAG_SYMT:
    case TAG_PRMT:
    {   reqeval ( state, nArity == 1
                , "Arity mismatch for atom -- need '%d', have '1'"
                , nArity);
        osRes[0] = oExp;
        return;
    }

    case TAG_VART:
    {   reqeval ( state, nArity == 1
                , "Arity mismatch for variable -- need '%d', have '1'"
                , nArity);

        obj_t* oRes
         = shivver_eval_resolve_var
                (oEnv, xVarT_name(oExp), xVarT_bump(oExp));

        reqeval ( state, oRes != 0
                , "Variable '%s' is not in scope."
                , xVarA_name(oExp));
        osRes[0] = oRes;
        return;
    }

    default:
    {   reqeval ( state, false
                , "Cannot evaluate object.");
    }
  }
}


