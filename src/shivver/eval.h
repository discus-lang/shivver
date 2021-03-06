#pragma once
#include <stdarg.h>
#include "shivver/heap.h"
#include "shivver/eval/types.h"


// from eval/prim.c
void
shivver_eval_prim
        ( eval_t*       state
        , obj_t*        oEnv
        , size_t        nArity
        , obj_t**       osRes
        , size_t        tag
        , size_t        nArgs
        , obj_t**       osArg);


// from eval/resolve.c
obj_t*
shivver_eval_resolve_var
        ( obj_t*        oEnv
        , char*         name
        , size_t        nBump);

obj_t*
shivver_eval_resolve_mac
        ( eval_decl_t*  decls
        , char*         name);

bool
shivver_eval_eqSym
        ( obj_t*        oSym
        , char*         name);


bool
shivver_eval_eqMac
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
shivver_eval_term1_null
        ( eval_t*       state
        , obj_t*        env
        , obj_t*        obj);

obj_t*
shivver_eval_term1
        ( eval_t*       state
        , obj_t*        env
        , obj_t*        obj);

void
shivver_force_termN
        ( eval_t*       state
        , size_t        nArity
        , obj_t**       osRes
        , obj_t*        oEnv
        , obj_t*        oExp);

void
shivver_eval_termN
        ( eval_t*       state
        , size_t        nArity
        , obj_t**       osRes
        , obj_t*        oEnv
        , obj_t*        oExp);
