#pragma once
#include "shivver/types.h"

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

