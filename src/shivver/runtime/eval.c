
#include "shivver/runtime.h"
#include "shivver/util.h"


obj_t*  shivver_eval (obj_t* oEnv, obj_t* oExp)
{
        obj_t*  oRes     = 0;
        shivver_evalN(1, &oRes, oEnv, oExp);
        return oRes;
}


void    shivver_evalN
        ( size_t  nArity        // Number of values we expect from evaluation.
        , obj_t** osRes         // Array of pointers to fill with results.
        , obj_t*  oEnv          // Evaluation environment.
        , obj_t*  oExp)         // Term expression to evaluate.
{ again:
        printf("* eval\n");
        shivver_printp(oExp);
        printf("\n\n");

        switch(xObj_tag(oExp))
        {
          // hot ----------------------------------------------------
          case TAG_MMMH:
          {     size_t nLen = xMmmH_len(oExp);

                // The arity of the vector must match that required by the context.
                if (nLen != nArity)
                        shivver_fail("arity mismatch in vector");

                // Evaluate each of the components in turn,
                // writing them into the result buffer.
                for (size_t i = 0; i < nLen; i++)
                        shivver_evalN(1, osRes + i, oEnv, xMmmH_arg(oExp, i));

                return;
          }

          case TAG_VARH:
          {     if (nArity != 1)
                        shivver_fail("arity mismatch for variable");

                if (shivver_resolveT
                        ( osRes
                        , oEnv, xVarH_name(oExp), xVarH_bump(oExp)))
                        return;

                printf("* variable out of scope\n");
                shivver_printp(oExp);
                shivver_fail("evaluation failed");
          }

          case TAG_SYMH:
          {     if (nArity != 1)
                        shivver_fail("arity mismtch for symbol");
                osRes[0] = oExp;
                return;
          }

          case TAG_PRMH:
          {     if (nArity != 1)
                        shivver_fail("arity mismatch for primitive");
                osRes[0] = oExp;
                return;
          }

          case TAG_ABSH:
          {     obj_t* oAbs     = oExp;
                obj_t* oBody    = xAbsH_body(oAbs);
                if (nArity != 1)
                        shivver_fail("arity mismatch for abstraction");

                osRes[0] = aCloH( xAbsH_len(oExp)
                                , oEnv, xAbsH_parmp(oExp), oBody);
                return;
          }

          case TAG_APPH:
          {     obj_t* oApp     = oExp;
                obj_t* oHead    = xAppH_fun(oApp);
                obj_t* oArg     = xAppH_arg(oApp);

                // Evaluate the head to a value.
                obj_t* oHeadV   = 0;
                shivver_evalN (1, &oHeadV, oEnv, oHead);

                // Decide what to do based on the form of the head.
                switch(xObj_tag(oHeadV))
                { case TAG_SYMH:
                  {     obj_t*  oArgV;
                        shivver_evalN(1, &oArgV, oEnv, oArg);
                        osRes[0]        = aAppH(oHeadV, oArgV);
                        return;
                  }

                  case TAG_CLOH:
                  {     // Unpack the components of the closure.
                        obj_t*  oClo    = oHeadV;
                        size_t  nParams = xCloH_len(oClo);
                        obj_t** osParms = xCloH_parmp(oClo);
                        obj_t*  oEnvClo = xCloH_env(oClo);
                        obj_t*  oBody   = xCloH_body(oClo);

                        printf("* eval params = %zu\n", nParams);

                        // Evaluate the arguments.
                        obj_t* osArgs[nParams];
                        shivver_evalN(nParams, osArgs, oEnv, oArg);

                        for (size_t i = 0; i < nParams; i++)
                        {       shivver_printp(osParms[i]);
                                shivver_printp(osArgs[i]);
                        }
                        printf("\n");

                        // Extend the closure environment with the function arguments.
                        obj_t*  oEnvExt = aEnvH(nParams, oEnvClo, osParms, osArgs);

                        // Tailcall ourselves to evaluate the body.
                        oEnv    = oEnvExt;
                        oExp    = oBody;
                        goto again;
                  }

                  default:
                        shivver_fail("cannot apply");
                }
          }

          // static  --------------------------------------------------
          case TAG_VART:
          {     if (nArity != 1)
                        shivver_fail("arity mismatch for variable");

                if (shivver_resolveT
                        ( osRes
                        , oEnv, xVarT_name(oExp), xVarT_bump(oExp)))
                        return;

                printf("* variable out of scope\n");
                shivver_printp(oExp);
                shivver_fail("evaluation failed");
          }

          case TAG_SYMT:
          {     if (nArity != 1)
                        shivver_fail("arity mismtch for symbol");
                osRes[0] = oExp;
                return;
          }

          case TAG_PRMT:
          {     if (nArity != 1)
                        shivver_fail("arity mismatch for primitive");
                osRes[0] = oExp;
                return;
          }

          default:
                shivver_printp(oExp);
                shivver_fail("don't know how to evaluate");

        }
}




// TODO: handle bump counter.
// Lookup a variable from the given environment.
//  If we find it set the outResult to the value and return true,
//  otherwise return false.
//
//  We treat an environemnt pointer of NULL as an empty environment,
//  and will always return false if given one.

bool    shivver_resolveT
        ( obj_t** outResult
        , obj_t*  oEnv, char* name, size_t bump)
{ again:
        if (oEnv == 0) return false;

        size_t nCount = xEnvH_len(oEnv);
        for (size_t i = 0; i < nCount; i++)
        {       obj_t* oParm = xEnvH_var(oEnv, i);
                if (shivver_eqSym (oParm, name))
                {       *outResult = xEnvH_val(oEnv, i);
                        return true;
                }
        }

        oEnv = xEnvH_parent(oEnv);
        goto again;
}


// Check if a symbol has the given name.
bool    shivver_eqSym
        (obj_t* oExp, char* name)
{
        switch(xObj_tag(oExp))
        { case TAG_VART:
                return strcmp(xVarT_name(oExp), name) == 0;

          case TAG_VARH:
                return strcmp(xVarH_name(oExp), name) == 0;

          default:
                printf("* eqSym: object is not a variable");
                shivver_printp(oExp);
                shivver_fail("evaluation failed");
        }
}

