
#include "shivver/runtime.h"

obj_t*  shivver_eval (obj_t* oEnv, obj_t* obj)
{ again:
        switch(xObj_tag(obj))
        { case TAG_MMM:
          {     size_t len = xMmm_len(obj);
                obj_t** buf = alloca(sizeof(obj_t*) * len);
                for (size_t i = 0; i < len; i++)
                        buf[i] = shivver_eval(oEnv, xMmm_arg(obj, i));
                return aMmm(len, buf);
          }

          case TAG_SYMT: return obj;

          case TAG_ABSM:
          {     return aCloM
                        ( xAbsM_len(obj)
                        , oEnv
                        , xAbsM_parms(obj)
                        , xAbsM_body(obj));
          }

          case TAG_APPV:
          {     obj_t* oFun  = xAppV_fun(obj);
                obj_t* oClo  = shivver_eval(oEnv, oFun);

                if (xObj_tag(oClo) != TAG_CLOM)
                {       printf("* not a closure\n");
                        shivver_printp(oClo);
                        printf("\n");
                        abort();
                }
                obj_t* oEnv  = xCloM_env(oClo);
                obj_t* oBody = xCloM_body(oClo);

                obj_t* oArg  = xAppV_arg(obj);
                obj_t* oRes  = shivver_eval(oEnv, oArg);

                switch (xObj_tag(oRes))
                { case TAG_MMM:
                  {     size_t nArity = xCloM_len(oClo);
                        if (xMmm_len(oRes) != nArity)
                        {   printf("* arity mismatch\n");
                            abort();
                        }
                        obj_t** osArgs  = xMmm_args(oRes);
                        obj_t** osParms = xCloM_parms(oClo);
                        obj_t*  oEnvClo = xCloM_env(oClo);
                        obj_t*  oEnvExt = aEnvM(nArity, oEnvClo, osParms, osArgs);

                        // Tail call ourselves, which is equivalent to:
                        //   return shivver_eval (oEnvExt, oBody);
                        oEnv = oEnvExt;
                        obj  = oBody;
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