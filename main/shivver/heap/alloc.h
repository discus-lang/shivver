#pragma once
#include "shivver/heap/types.h"
#include "shivver/error.h"

static inline uint64_t*
halloc (size_t nWords)
{
        if (shivver_heap.last - shivver_heap.next < nWords)
                shivver_fail("halloc: out of space");
        uint64_t* obj     = shivver_heap.next;
        shivver_heap.next += nWords;
        return obj;
}

