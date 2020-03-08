#pragma once
#include "shivver/runtime.h"
#include "shivver/util.h"


static inline uint64_t*
halloc (size_t nWords)
{
        if (shivver_heap.last - shivver_heap.next < nWords)
                shivver_fail("halloc: out of space");
        uint64_t* obj     = shivver_heap.next;
        shivver_heap.next += nWords;
        return (void*)obj;
}


