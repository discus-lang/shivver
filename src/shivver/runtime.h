#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shivver/runtime/types.h"
#include "shivver/runtime/heap.h"
#include "shivver/runtime/object.h"
#include "shivver/runtime/object/base.h"
#include "shivver/runtime/object/atomic.h"
#include "shivver/runtime/object/static.h"
#include "shivver/runtime/object/hot.h"



// ----------------------------------------------------------------------------
// inlines

void    shivver_heapInit(size_t nWords);


// from eval.c
obj_t*
shivver_eval
        (obj_t* env, obj_t* obj);

void
shivver_evalN
        (size_t         nArity
        , obj_t**       osRes
        , obj_t*        oEnv
        , obj_t*        oExp);

obj_t*
shivver_resolveT
        ( obj_t*        oEnv
        , char*         name
        , size_t        nBump);

bool    shivver_eqSym
        (obj_t* oSym, char* name);
