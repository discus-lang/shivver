#include "shivver/runtime.h"

void
shivver_heapInit(size_t nWords)
{
        size_t nBytes     = sizeof(uint64_t) * nWords;
        shivver_heap.size = nBytes;
        shivver_heap.base = malloc(nBytes);
        shivver_heap.next = shivver_heap.base;
        shivver_heap.last = shivver_heap.base + nWords;
}