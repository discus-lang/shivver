#pragma once
#include <stdlib.h>
#include <stdint.h>

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
// Generic heap object representation.
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
#define TAG_SYMT        0x01
#define TAG_VART        0x05

// ----------------------------------------------------------------------------
// inlines
#include "shivver/runtime/heap.h"
#include "shivver/runtime/alloc.h"

void    shivver_heapInit(size_t nWords);