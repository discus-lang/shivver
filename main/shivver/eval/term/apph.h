#pragma once
#include "shivver/eval.h"
#include "shivver/eval/error.h"
#include "shivver/prim.h"


// Application to a general term expression.
//   In the case where we want to continue evaluating the body of an
//   an abstraction, we use a trampoline mechanism to avoid recursively
//   calling our caller 'shivver_eval_termN'. We take references to the
//   current environment and expression pointer, which can be overwritten
//   with a new environment and body of the abstraction. Returning 'true'
//   then instructs our caller to perform a tail-call with these
//   new pointers.
//
static inline bool
shivver_eval_termN_AppH
        ( eval_t* state
        , size_t  nRes,  obj_t **osRes
        , obj_t** opEnv, obj_t **opExp)
{
        // Get the direct environment and expression pointers.
        obj_t* oEnv     = *opEnv;
        obj_t* oExp     = *opExp;

        // Split out the head and argument pointers.
        obj_t* oApp     = oExp;
        obj_t* oHead    = xAppH_fun(oApp);
        obj_t* oArg     = xAppH_arg(oApp);

        // Evaluate the head.
        obj_t* oHeadV   = 0;
        shivver_force_termN
                ( state, 1, &oHeadV, oEnv, oHead);

        // What to do next is determined by the form of the head value.
        switch(xObj_tag(oHeadV))
        {
          // For applications of symbols,
          //   evaluate all the arguments and rebuild the application.
          case TAG_SYMA:
          {     obj_t*  oArgV   = 0;
                shivver_eval_termN (state, 1, &oArgV, oEnv, oArg);
                osRes[0] = aAppH(oHeadV, oArgV);
                return false;
          }

          // For applications of primitives,
          //   evaluate all the arguments and call the corresponding prim function.
          case TAG_PRZA:
          {     obj_t*   oPrzA  = oHeadV;
                uint32_t pTag   = xPrzA_tag(oPrzA);

                // Evaluate all the arguments.
                //   For the non-vector application form we need to know how many
                //   arguments the prim takes so that we know how many slots to
                //   allocate for argument pointers here on the stack.
                size_t nParams  = shivver_prim_tag_args(pTag);
                reqeval ( state, nParams != 0xff
                        , "Cannot apply arity polymorphic primitive to non-vector argument.");

                // Allocate space for the argument pointers,
                // and then evaluate the arguments themselves.
                obj_t* osArgs[nParams];
                shivver_eval_termN (state, nParams, osArgs, oEnv, oArg);

                // Call the primitive function, giving it the evaluated arguments.
                shivver_eval_prim  (state, oEnv, nRes, osRes, pTag, nParams, osArgs);
                return false;
          }

          // For applications of closures,
          //   evaluate the arguments, add them to environment and continue
          //   with the body expresssion.
          case TAG_CLOH:
          {     obj_t*  oClo    = oHeadV;
                size_t  nParams = xCloH_len(oClo);

                // Evaluate the argument to produce a result vector.
                obj_t* osArgs[nParams];
                shivver_eval_termN (state, nParams, osArgs, oEnv, oArg);

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
