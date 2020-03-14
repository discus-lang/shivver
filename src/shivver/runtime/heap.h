#pragma once
#include "shivver/runtime/error.h"
#include "shivver/runtime/heap/tag.h"

// ----------------------------------------------------------------------------
// Runtime heap space.
typedef struct {
        uint64_t  size;
        uint64_t* base;
        uint64_t* next;
        uint64_t* last;
} heap_t;


// ----------------------------------------------------------------------------
// Generic heap object representation
typedef struct {
        uint64_t header;
        uint64_t payload[];
} obj_t;


// ----------------------------------------------------------------------------
// heap
extern heap_t shivver_heap;


// ----------------------------------------------------------------------------
static inline uint64_t*
halloc (size_t nWords)
{
        if (shivver_heap.last - shivver_heap.next < nWords)
                shivver_fail("halloc: out of space");
        uint64_t* obj     = shivver_heap.next;
        shivver_heap.next += nWords;
        return (void*)obj;
}

