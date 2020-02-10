#pragma once
#include <stdint.h>

typedef uint32_t uint24_t;


// ----------------------------------------------------------------------------
// Runtime heap space.
typedef struct {
        uint64_t  size;
        uint64_t  top;
        uint64_t* data;
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
#define TAG_SYMT        0x01
#define TAG_VART        0x05;


// ----------------------------------------------------------------------------
// from heap.h
static inline uint64_t*
hAlloc (size_t nWords);


// ----------------------------------------------------------------------------
// from alloc.h
static inline obj_t*
aSymT (const char* name);

static inline obj_t*
aVarT (const char* name, uint24_t nBump);

