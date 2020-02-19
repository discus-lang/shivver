
#include "shivver/runtime.h"

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

          case TAG_SYMT: return obj;

          case TAG_ABSH:
          {     return aCloH
                        ( xAbsH_len(obj)
                        , oEnv
                        , xAbsH_parms(obj)
                        , xAbsH_body(obj));
          }

          case TAG_APVH:
          {     // Evaluate the function part of the application to a closure.
                obj_t* oFun  = xApvH_fun(obj);
                obj_t* oClo  = shivver_eval(oEnv, oFun);
                if (xObj_tag(oClo) != TAG_CLOH)
                {       printf("* not a closure\n");
                        shivver_printp(oClo);
                        printf("\n");
                        abort();
                }

                // Evaluate the argument part of the application to a result.
                obj_t* oArg  = xApvH_arg(obj);
                obj_t* oRes  = shivver_eval(oEnv, oArg);

                switch (xObj_tag(oRes))
                { // Result is a vector.
                  case TAG_MMMH:
                  {     // Ariry of function must match number of values in the vector.
                        size_t nArity = xCloH_len(oClo);
                        if (xMmmH_len(oRes) != nArity)
                        {   printf("* arity mismatch\n");
                            abort();
                        }

                        // Extend the closure environment with the function arguments.
                        obj_t** osArgs  = xMmmH_args(oRes);
                        obj_t** osParms = xCloH_parms(oClo);
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
                        abort();
                }
          }

          default:
                return obj;
        }
}