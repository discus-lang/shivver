
#include "shivver/runtime.h"
#include "shivver/prim.h"
#include "shivver/util.h"

static inline void
reqeval (bool prop, char* message)
{       require(prop, message);
}


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
        switch(xObj_tag(oExp))
        {
          // atomic -------------------------------------------------
          case TAG_VARA:
          {     reqeval ( nArity == 1
                        , "eval arity for variable must be one");

                obj_t* oRes
                 = shivver_resolveT
                        (oEnv, xVarA_name(oExp), xVarA_bump(oExp));

                reqeval ( oRes != 0
                        , "variable out of scope");

                osRes[0] = oRes;
                return;
          }

          case TAG_SYMA:
          {     reqeval ( nArity == 1
                        , "eval arity for symbol must be one");

                osRes[0] = oExp;
                return;
          }

          case TAG_PRMA:
          {     reqeval ( nArity == 1
                        , "eval arity for primitive must be one");

                osRes[0] = oExp;
                return;
          }

          case TAG_PRZA:
          {     reqeval ( nArity == 1
                        , "eval arity for primitive must be one");

                osRes[0] = oExp;
                return;
          }

          case TAG_NATA:
          {     reqeval ( nArity == 1
                        , "eval arity for literal must be one");
                osRes[0] = oExp;
                return;
          }

          // hot ----------------------------------------------------
          case TAG_MMMH:
          {     size_t nLen = xMmmH_len(oExp);
                reqeval ( nLen == nArity
                        , "eval arity does not match vector length");

                for (size_t i = 0; i < nLen; i++)
                        shivver_evalN(1, osRes + i, oEnv, xMmmH_arg(oExp, i));

                return;
          }

          case TAG_ABSH:
          {     // Convert abstractions into closures.
                reqeval ( nArity == 1
                        , "eval arity for abstraction must be one");

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
                shivver_evalN (1, &oHeadV, oEnv, oHead);

                switch(xObj_tag(oHeadV))
                { case TAG_SYMA:
                  {     // For applications of symbols,
                        // evaluate all the arguments and rebuild the application.
                        obj_t*  oArgV;
                        shivver_evalN(1, &oArgV, oEnv, oArg);
                        osRes[0] = aAppH(oHeadV, oArgV);
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
                        shivver_evalN(nParams, osArgs, oEnv, oArg);

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
                shivver_evalN (1, &oHeadV, oEnv, oHead);

                switch(xObj_tag(oHeadV))
                { case TAG_SYMA:
                  {     // For applications of symbols,
                        // evaluate all the arguments and rebuild the application.
                        obj_t* osArgVs[nArgs];
                        for(size_t i = 0; i < nArgs; i++)
                        {       obj_t* oArg = xApsH_arg(oAps, i);
                                shivver_evalN(1, osArgVs + i, oEnv, oArg);
                        }

                        osRes[0] = aApsH(nArgs, oHeadV, osArgVs);
                        return;
                  }

                  case TAG_CLOH:
                  {     // For applications of closures,
                        // evaluate the arguments, add them to environment and continue
                        // with the body expresssion.
                        obj_t* oClo     = oHeadV;
                        size_t nParams  = xCloH_len(oClo);
                        size_t nArgs    = xApsH_len(oAps);
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
                                        shivver_evalN(1, osArgs + i, oEnv, oArg);
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
          case TAG_VART:
          {     reqeval ( nArity == 1
                        , "eval arity for variable must be one");

                obj_t* oRes
                 = shivver_resolveT
                        (oEnv, xVarT_name(oExp), xVarT_bump(oExp));

                reqeval ( oRes != 0
                        , "variable out of scope");

                osRes[0] = oRes;
                return;
        }

          case TAG_SYMT:
          {     reqeval ( nArity == 1
                        , "eval arity for symbol must be one");
                osRes[0] = oExp;
                return;
          }

          case TAG_PRMT:
          {     reqeval ( nArity == 1
                        , "eval arity for primitive must be one");
                osRes[0] = oExp;
                return;
          }

          default:
                shivver_prim_console_printp(oExp);
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
//
obj_t*
shivver_resolveT
        (obj_t* oEnv, char* name, size_t bump)
{ again:
        if (oEnv == 0)
                return 0;

        size_t nCount = xEnvH_len(oEnv);
        for (size_t i = 0; i < nCount; i++)
        {       obj_t* oParm = xEnvH_var(oEnv, i);
                if (shivver_eqSym (oParm, name))
                {       return xEnvH_val(oEnv, i);
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

          case TAG_VARA:
                return strcmp(xVarA_name(oExp), name) == 0;

          default:
                printf("* eqSym: object is not a variable");
                shivver_prim_console_printp(oExp);
                shivver_fail("evaluation failed");
        }
}

