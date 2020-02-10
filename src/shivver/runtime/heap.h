#pragma once
#include "shivver/runtime.h"


static inline uint64_t*
halloc (size_t nWords)
{
        if (shivver_heap.last - shivver_heap.next < nWords) abort();
        uint64_t* obj     = shivver_heap.next;
        shivver_heap.next += nWords;
        return (void*)obj;
}


