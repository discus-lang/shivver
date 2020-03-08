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


// ----------------------------------------------------------------------------
// heap
heap_t shivver_heap;


// ----------------------------------------------------------------------------
// Objects
//                                      sort    mode
//         7  6  5  4  3  2  1  0       ------- ---
//  mmmh   len.ptrs... 0  0  0  F       0 0 0 0 1 1 0 1  = 0x0d
//  varh   len.chars.. 0  0  0  F       0 0 0 1 1 1 0 1  = 0x1d
//  symh   len.chars.. 0  0  0  F       0 0 1 0 1 1 0 1  = 0x2d
//  prmh   len.chars.. 0  0  0  F       0 0 1 1 1 1 0 1  = 0x3d
//  absh   len.ptrs... 0  0  0  F       0 1 0 0 1 1 0 1  = 0x4d
//  apvh   0  0  0  0  0  0  0  F       0 1 0 1 1 1 0 1  = 0x5d
//  apsh   len.ptrs... 0  0  0  F       0 1 1 0 1 1 0 1  = 0x6d
//  cloh   len.ptrs... 0  0  0  F       1 0 0 0 1 1 0 1  = 0x8d
//  envh   len.ptrs... 0  0  0  F       1 0 0 1 1 1 0 1  = 0x9d

//  symt   0  0  0  0  bump  .  F       0 0 0 1 0 0 0 1  = 0x11
//  vart   0  0  0  0  bump  .  F       0 0 1 0 0 0 0 1  = 0x21
//  prmt   0  0  0  0  bump  .  F       0 0 1 1 0 0 0 1  = 0x31
//
//  mode: 00 static
//        01 cold
//        11 hot
//
//  sort: 0000 mmm
//        0001 var
//        0010 sym
//        0011 prm
//        0100 abs
//        0101 app
//        0110 aps
//        1000 clo
//        1001 env
//

#define TAG_MMMH        0x0d
#define TAG_VARH        0x1d
#define TAG_SYMH        0x2d
#define TAG_PRMH        0x3d
#define TAG_ABSH        0x4d
#define TAG_APPH        0x5d
#define TAG_APSH        0x6d
#define TAG_CLOH        0x8d
#define TAG_ENVH        0x9d

#define TAG_VART        0x11
#define TAG_SYMT        0x21
#define TAG_PRMT        0x31


// ----------------------------------------------------------------------------
// inlines
#include "shivver/runtime/heap.h"
#include "shivver/runtime/object/base.h"
#include "shivver/runtime/object/hot.h"
#include "shivver/runtime/object/static.h"


void    shivver_heapInit(size_t nWords);


// from printl.c
void    shivver_printl(obj_t* obj);

// from printp.c
void    shivver_printp(obj_t* obj);

// from printm.c
void    shivver_printm(obj_t* obj);

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
