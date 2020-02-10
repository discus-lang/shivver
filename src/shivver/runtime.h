#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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
// heap
heap_t shivver_heap;


// ----------------------------------------------------------------------------
// Objects
//
//         7  6  5  4  3  2  1  0
//  aMmm   count .  .  0  0  0  F       0 0 0 0 0 0 0 1  = 0x01
//  symt   0  0  0  0  bump  .  F       0 0 0 0 1 0 0 1  = 0x09
//  vart   0  0  0  0  bump  .  F       0 0 0 0 1 1 0 1  = 0x0d
//  absm   count .  .  0  0  0  F       0 0 0 0 1 0 1 1  = 0x0b
//  clom   count .  .  0  0  0  F       0 0 0 1 1 0 1 1  = 0x1b
//  apsh   count .  .  0  0  0  F       0 0 0 0 1 1 1 1  = 0x0f
//  apvs   0  0  0  0  0  0  0  F       0 0 0 1 1 1 1 1  = 0x1f
//  envm   count .  .  0  0  0  F       0 0 1 1 1 1 1 1  = 0x3f

#define TAG_MMMH        0x01
#define TAG_SYMT        0x09
#define TAG_VART        0x0d
#define TAG_ABSH        0x0b
#define TAG_CLOH        0x1b
#define TAG_APSH        0x0f
#define TAG_APVH        0x1f
#define TAG_ENVH        0x3f


// ----------------------------------------------------------------------------
// inlines
#include "shivver/runtime/heap.h"
#include "shivver/runtime/alloc.h"

void    shivver_heapInit(size_t nWords);


// from printl.c
void    shivver_printl(obj_t* obj);

// from printp.c
void    shivver_printp(obj_t* obj);

// from eval.c
obj_t*  shivver_eval(obj_t* env, obj_t* obj);
