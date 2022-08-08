#pragma once
#include "shivver/eval.h"
#include "shivver/eval/error.h"


// Atoms are already evaluated, so we return them unharmed.
static inline void
shivver_eval_termN_Atom
        ( eval_t* state
        , size_t  nRes, obj_t** osRes
        , obj_t*  oEnv, obj_t*  oExp)
{
        reqeval ( state, nRes == 1
                , "Arity mismatch for atom -- need '%d', have '1'."
                , nRes);

        osRes[0] = oExp;
}


// Resolve am atomic variable to its bound expression.
static inline void
shivver_eval_termN_VarA
        ( eval_t* state
        , size_t  nRes, obj_t** osRes
        , obj_t*  oEnv, obj_t*  oExp)
{
        reqeval ( state, nRes == 1
                , "Arity mismatch for variable -- need '%d', have '1'."
                , nRes);

        obj_t* oRes
         = shivver_eval_resolve_var
                (oEnv, xVarA_name(oExp), xVarA_bump(oExp));

        reqeval ( state, oRes != 0
                , "Variable '%s' is not in scope."
                , xVarA_name(oExp));

        osRes[0] = oRes;
}


// Resolve a static variable to its bound expression.
static inline void
shivver_eval_termN_VarT
        ( eval_t* state
        , size_t  nRes, obj_t** osRes
        , obj_t*  oEnv, obj_t*  oExp)
{
        reqeval ( state, nRes == 1
                , "Arity mismatch for variable -- need '%d', have '1'"
                , nRes);

        obj_t* oRes
         = shivver_eval_resolve_var
                (oEnv, xVarT_name(oExp), xVarT_bump(oExp));

        reqeval ( state, oRes != 0
                , "Variable '%s' is not in scope."
                , xVarA_name(oExp));
        osRes[0] = oRes;
        return;
}


// Evaluate all the components of a term vector.
static inline void
shivver_eval_termN_MmmH
        ( eval_t* state
        , size_t  nRes, obj_t** osRes
        , obj_t*  oEnv, obj_t*  oExp)
{
        size_t nLen = xMmmH_len(oExp);
        reqeval ( state, nLen == nRes
                , "Arity mismatch for term vector -- need '%d', have '%d'."
                , nRes, nLen);

        for (size_t i = 0; i < nLen; i++)
         shivver_eval_termN
                ( state, 1, osRes + i
                , oEnv, xMmmH_arg(oExp, i));
}


// Convert abstractions to closures.
static inline void
shivver_eval_termN_AbsH
        ( eval_t* state
        , size_t  nRes, obj_t** osRes
        , obj_t*  oEnv, obj_t*  oExp)
{
        reqeval ( state, nRes == 1
                , "Arity mismatch for abstraction -- need '%d', have '1'."
                , nRes);

        osRes[0]
         = aCloH ( xAbsH_len(oExp)
                 , oEnv, xAbsH_parmp(oExp), xAbsH_body(oExp));
}
