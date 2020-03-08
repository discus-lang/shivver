
#include "shivver/runtime.h"
#include "shivver/util.h"

obj_t*  shivver_eval (obj_t* oEnv, obj_t* obj)
{ again:
        switch(xObj_tag(obj))
        { case TAG_MMMH:
          {     size_t len = xMmmH_len(obj);
                obj_t** buf = alloca(sizeof(obj_t*) * len);
                for (size_t i = 0; i < len; i++)
                        buf[i] = shivver_eval(oEnv, xMmmH_arg(obj, i));
                return aMmmH(len, buf);
          }

          case TAG_SYMT:
                return obj;

          case TAG_VART:
          {     obj_t*  oVal = 0;
                if (shivver_resolveT
                        ( oEnv
                        , xVarT_name(obj)
                        , xVarT_bump(obj)
                        , &oVal))
                        return oVal;
                printf("* variable out of scope\n");
                shivver_printp(obj);
                shivver_fail("evaluation failed");
          }

          case TAG_ABSH:
          {     obj_t* oClo
                 = aCloH
                        ( xAbsH_len(obj)
                        , oEnv
                        , xAbsH_parmp(obj)
                        , xAbsH_body(obj));

                return oClo;
          }

          case TAG_APPH:
          {     // Evaluate the function part of the application to a closure.
                obj_t* oFun  = xAppH_fun(obj);
                obj_t* oClo  = shivver_eval(oEnv, oFun);
                if (xObj_tag(oClo) != TAG_CLOH)
                {       printf("* not a closure\n");
                        shivver_printp(oClo);
                        printf("\n");
                        shivver_fail("evaluation failed");
                }

                // Evaluate the argument part of the application to a result.
                obj_t* oArg  = xAppH_arg(obj);
                obj_t* oRes  = shivver_eval(oEnv, oArg);

                switch (xObj_tag(oRes))
                { // Result is a vector.
                  case TAG_MMMH:
                  {     // Arity of function must match number of values in the vector.
                        size_t nArity = xCloH_len(oClo);
                        if (xMmmH_len(oRes) != nArity)
                        {       printf("* arity mismatch\n");
                                shivver_fail("evaluation failed");
                        }

                        // Extend the closure environment with the function arguments.
                        obj_t** osArgs  = xMmmH_args(oRes);
                        obj_t** osParms = xCloH_parmp(oClo);
                        obj_t*  oEnvClo = xCloH_env(oClo);
                        obj_t*  oEnvExt = aEnvH(nArity, oEnvClo, osParms, osArgs);

                        // Evalute the body of the closure.
                        //   We tail-call the evaluation function,
                        //   which is equivalent to shivver_eval (oEnvExt, oBody);
                        oEnv = oEnvExt;
                        obj  = xCloH_body(oClo);
                        goto again;
                  }
                  default:
                        shivver_fail("eval: unhandled application");
                }
          }

          default:
                return obj;
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
        ( obj_t* oEnv, char* name, size_t bump
        , obj_t** outResult)
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
        (obj_t* oSym, char* name)
{
        switch(xObj_tag(oSym))
        { case TAG_SYMT:
                if (strcmp(xSymT_name(oSym), name) == 0)
                        return true;
                return false;

          default:
                printf("* eqSym: object is not a symbol");
                shivver_printp(oSym);
                shivver_fail("evaluation failed");
        }
}

