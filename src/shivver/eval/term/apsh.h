#pragma once
#include "shivver/eval.h"
#include "shivver/eval/error.h"
#include "shivver/prim.h"


// Application to a vector of term expressions.
static inline bool
shivver_eval_termN_ApsH
        ( eval_t* state
        , size_t  nRes,  obj_t** osRes
        , obj_t** opEnv, obj_t** opExp)
{
        // Get the direct environment and expression pointers.
        obj_t* oEnv     = *opEnv;
        obj_t* oExp     = *opExp;

        // split out the head term and argument length.
        obj_t* oAps     = oExp;
        obj_t* oHead    = xApsH_fun(oAps);
        uint32_t nArgs  = xApsH_len(oAps);

        // Evaluate the head.
        obj_t* oHeadV   = 0;
        shivver_force_termN
                (state, 1, &oHeadV, oEnv, oHead);

        // What to do next is determined by the form of the head value.
        switch(xObj_tag(oHeadV))
        {
          // For applications of symbols,
          // evaluate all the arguments and rebuild the application.
          case TAG_SYMA:
          {     // Evaluate the arguments.
                obj_t* osArgVs[nArgs];
                for(size_t i = 0; i < nArgs; i++)
                  shivver_eval_termN
                        (state, 1, osArgVs + i, oEnv, xApsH_arg(oAps, i));

                // Rebuild the application.
                osRes[0] = aApsH(nArgs, oHeadV, osArgVs);
                return false;
          }

          // For applications of primitives,
          //   evaluate all arguments and call the corresponding prim function.
          case TAG_PRZA:
          {     // Evaluate all the arguments.
                obj_t* osArgs[nArgs];
                for(size_t i = 0; i < nArgs; i++)
                  shivver_eval_termN
                        (state, 1, osArgs + i, oEnv, xApsH_arg(oAps, i));

                // Apply the primitive.
                shivver_eval_prim
                        ( state,  oEnv
                        , nRes,   osRes
                        , xPrzA_tag(oHeadV)
                        , nArgs,  osArgs);
                return false;
          }

          // For applications of closures,
          //   evaluate the arguments, add them to environment and continue
          //   with the body expresssion.
          case TAG_CLOH:
          {     obj_t* oClo     = oHeadV;
                size_t nParams  = xCloH_len(oClo);
                reqeval ( state, nParams == nArgs
                        , "Arity mismatch in function application -- need '%d', have '%d'"
                        , nParams, nArgs);

                // Application of closure with no parameters.
                //   so we have ({} Term) []
                //   We don't need to allocate a new environment frame.
                if (nParams == 0)
                {       oExp    = xCloH_body(oClo);
                        return true;
                }

                // Application of a closure with at least one parameter.
                // Evaluate the arguments individually.
                obj_t* osArgs[nArgs];
                for(size_t i = 0; i < nArgs; i++)
                  shivver_eval_termN
                        (state, 1, osArgs + i, oEnv, xApsH_arg(oAps, i));

                // Update the current environment and expression pointers,
                // and return 'true' to tell the caller to tail-call itself
                // with this new configuration. We avoid directly calling
                // 'shivver_eval_termN' again to avoid building a new C stack frame.
                *opEnv = aEnvH ( nParams
                               , xCloH_env(oClo)
                               , xCloH_parmp(oClo), osArgs);

                *opExp = xCloH_body(oClo);
                return true;
          }

          // Primitive name that we don't have an eval function for,
          // or hasn't been mapped to a builtin prz identifier.
          case TAG_PRMA:
                reqeval ( state, false
                        , "Unknown primitive '#%s'.", xPrmA_name(oHeadV));

          // We don't know how to apply the head expression.
          // Maybe it was a literal or some other non-functional value.
          default:
                reqeval ( state, false
                        , "Cannot apply non-functional value.");
        }
        assert(false);
}
