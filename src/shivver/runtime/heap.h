
#include ""

static inline uint64_t*
halloc (size_t nWords)
{
        uint64_t* obj   = heap.top;
        heap.top += nWords;
        return (void*)obj;
}


