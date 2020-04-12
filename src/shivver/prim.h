#pragma once
#include "shivver/prim/tag.h"
#include "shivver/prim/console.h"


// Get the name of the primitive with the given tag.
char*
shivver_prim_nameOfTag
        (uint32_t tag);


// Get the tag of the primitive with the given name.
uint32_t
shivver_prim_tagOfName
        (char* name);


// Get the primitive block number from a tag.
static inline size_t
shivver_prim_tag_block
        (uint32_t tag)
{       return (tag >> 24) & 0xff;
}


// Get the primitive number with the block from a tag.
static inline size_t
shivver_prim_tag_prim
        (uint32_t tag)
{       return (tag >> 16) & 0xff;
}


// Get the number of primitive arguments from a tag.
static inline size_t
shivver_prim_tag_args
        (uint32_t tag)
{       return (tag >> 8) & 0xff;
}


// Get the number of primitive results from a tag.
static inline size_t
shivver_prim_tag_results
        (uint32_t tag)
{       return tag & 0xff;
}

