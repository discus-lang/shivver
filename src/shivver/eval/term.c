
#include <assert.h>
#include "shivver/eval.h"
#include "shivver/prim.h"
#include "shivver/util.h"

static inline void
reqeval (bool prop, char* message)
{       require(prop, message);
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
        {
          // atomic -------------------------------------------------
          case TAG_SYMA:
          case TAG_PRMA:
          case TAG_PRZA:
          case TAG_NATA:
          {     reqeval (nArity == 1,   "eval arity for atom must be one");
                osRes[0] = oExp;
                return;
          }

          case TAG_VARA:
          {     reqeval (nArity == 1,   "eval arity for variable must be one");
                obj_t* oRes
                 = shivver_eval_resolve
                        (oEnv, xVarA_name(oExp), xVarA_bump(oExp));

                shivver_eval_require
                        ( state, oRes != 0
                        , "Variable '%s' not in scope."
                        , xVarA_name(oExp));
                osRes[0] = oRes;
                return;
          }

          // hot ----------------------------------------------------
          case TAG_MMMH:
          {     size_t nLen = xMmmH_len(oExp);
                reqeval (nLen == nArity, "eval arity does not match vector length");
                for (size_t i = 0; i < nLen; i++)
                        shivver_eval_termN
                                ( state, 1, osRes + i
                                , oEnv, xMmmH_arg(oExp, i));
                return;
          }

          case TAG_ABSH:
          {     // Convert abstractions into closures.
                reqeval ( nArity == 1,  "eval arity for abstraction must be one");
                obj_t* oAbs     = oExp;
                obj_t* oBody    = xAbsH_body(oAbs);
                osRes[0]        = aCloH ( xAbsH_len(oExp)
                                        , oEnv, xAbsH_parmp(oExp), oBody);
                return;
          }

          case TAG_APPH:
          {     obj_t* oApp     = oExp;
                obj_t* oHead    = xAppH_fun(oApp);
                obj_t* oArg     = xAppH_arg(oApp);

                // Evaluate the head.
                obj_t* oHeadV   = 0;
                shivver_eval_termN
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
                        size_t nParams  = shivver_prim_tag_args(pTag);

                        obj_t* osArgs[nParams];
                        shivver_eval_termN (state, nParams, osArgs, oEnv, oArg);

                        size_t nResults = shivver_prim_tag_results(pTag);
                        reqeval ( nArity == nResults
                                , "eval arity does not match number of prim op results");

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

                  default:
                        shivver_fail("cannot apply value");
                }
          }

          case TAG_APSH:
          {     obj_t* oAps     = oExp;
                uint32_t nArgs  = xApsH_len(oAps);
                obj_t* oHead    = xApsH_fun(oAps);

                // Evaluate the head.
                obj_t* oHeadV   = 0;
                shivver_eval_termN (state, 1, &oHeadV, oEnv, oHead);

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
                        size_t nParams  = shivver_prim_tag_args(pTag);
                        reqeval ( nParams == nArgs
                                , "number of parameters must match number of arguments");

                        // Evaluate all the arguments.
                        obj_t* osArgs[nArgs];
                        for(size_t i = 0; i < nArgs; i++)
                        {       obj_t* oArg = xApsH_arg(oAps, i);
                                shivver_eval_termN (state, 1, osArgs + i, oEnv, oArg);
                        }

                        // Apply the primitive.
                        size_t nResults = shivver_prim_tag_results(pTag);
                        reqeval ( nArity == nResults
                                , "eval arity does not match number of prim op results");
                        shivver_eval_prim (nArity, osRes, pTag, osArgs);
                        return;
                  }

                  case TAG_CLOH:
                  {     // For applications of closures,
                        // evaluate the arguments, add them to environment and continue
                        // with the body expresssion.
                        obj_t* oClo     = oHeadV;
                        size_t nParams  = xCloH_len(oClo);
                        reqeval ( nParams == nArgs
                                , "number of parameters must match number of arguments");

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

                  default:
                        shivver_fail("cannot apply value");
                }
          }


          // static  --------------------------------------------------
          case TAG_SYMT:
          case TAG_PRMT:
          {     reqeval ( nArity == 1,  "eval arity for atom must be one");
                osRes[0] = oExp;
                return;
          }

          case TAG_VART:
          {     reqeval ( nArity == 1,  "eval arity for variable must be one");

                obj_t* oRes
                 = shivver_eval_resolve
                        (oEnv, xVarT_name(oExp), xVarT_bump(oExp));
                reqeval ( oRes != 0,    "variable out of scope");

                osRes[0] = oRes;
                return;
          }

          default:
                shivver_prim_console_printp(oExp);
                shivver_fail("don't know how to evaluate");

        }
}


