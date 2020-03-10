#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint32_t uint24_t;


// ----------------------------------------------------------------------------
// Runtime heap space.
typedef struct {
        uint64_t  size;
        uint64_t* base;
        uint64_t* next;
        uint64_t* last;
} heap_t;


// ----------------------------------------------------------------------------
// Generic heap object represen tation.
typedef struct {
        uint64_t header;
        uint64_t payload[];
} obj_t;

typedef struct {
        uint64_t header;
        char*    string;
} obj_symt_t;

typedef struct {
        uint64_t header;
        char*    string;
} obj_vart_t;


// ----------------------------------------------------------------------------
void
shivver_fail
        (char* reason)
        __attribute__((noreturn));

static inline void
require ( bool  prop
        , char* message)
{
        if (!prop) shivver_fail(message);
}


// ----------------------------------------------------------------------------
// heap
heap_t shivver_heap;


// ----------------------------------------------------------------------------
// inlines
#include "shivver/runtime/heap.h"
#include "shivver/runtime/tag.h"
#include "shivver/runtime/object/base.h"
#include "shivver/runtime/object/atomic.h"
#include "shivver/runtime/object/static.h"
#include "shivver/runtime/object/hot.h"


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
