#pragma once
#include "shivver/heap.h"




// from eval/resolve.c
obj_t*
shivver_eval_resolveT
        ( obj_t*        oEnv
        , char*         name
        , size_t        nBump);

bool
shivver_eval_eqSym
        ( obj_t*        oSym
        , char*         name);


// from eval/prim.c
void
shivver_eval_prim
        ( size_t        nArity
        , obj_t**       osRes
        , size_t        tag
        , obj_t**       osArg);


// from eval/term.c
obj_t*
shivver_eval
        ( obj_t*        env
        , obj_t*        obj);

void
shivver_evalN
        ( size_t        nArity
        , obj_t**       osRes
        , obj_t*        oEnv
        , obj_t*        oExp);
