#pragma once
#include <stdarg.h>
#include "shivver/heap.h"
#include "shivver/eval/types.h"


// from eval/prim.c
void
shivver_eval_prim
        ( size_t        nArity
        , obj_t**       osRes
        , size_t        tag
        , obj_t**       osArg);


// from eval/resolve.c
obj_t*
shivver_eval_resolve
        ( obj_t*        oEnv
        , char*         name
        , size_t        nBump);

bool
shivver_eval_eqSym
        ( obj_t*        oSym
        , char*         name);


// from state.c
eval_t*
shivver_eval_alloc
        ();

void
shivver_eval_free
        ( eval_t*       state);

void
shivver_eval_require
        ( eval_t*       state
        , bool          prop
        , char*         format
        , ...);

void
shivver_eval_error
        ( eval_t*       state
        , char*         format
        , va_list       args);

void
shivver_eval_ingest
        ( eval_t*       state
        , obj_t*        oModule);


// from eval/term.c
obj_t*
shivver_eval_term_zero
        ( eval_t*       state
        , obj_t*        env
        , obj_t*        obj);

obj_t*
shivver_eval_term1
        ( eval_t*       state
        , obj_t*        env
        , obj_t*        obj);

void
shivver_eval_termN
        ( eval_t*       state
        , size_t        nArity
        , obj_t**       osRes
        , obj_t*        oEnv
        , obj_t*        oExp);
