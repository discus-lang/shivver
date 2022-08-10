#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


// A block of storage space including the size and next allocation offset.
typedef struct sv_store_block_t_ {
        // Size of the usable space, in bytes.
        size_t  size;

        // Index of the next byte to allocate.
        size_t  next;

        // Link ot the next block, or 0 if this is the last block.
        struct sv_store_block_t_* link;

        // Storage space in the block.
        uint8_t space[];
} sv_store_block_t;


// A region of the store constructed from a chain of blocks.
//  We incrementally allocate large numbers of small objects into a single
/// region then free the whole region when it is no longer needed.
typedef struct {
        // Block size, in bytes.
        //  This is the size of the usable space,
        //  not including the block header.
        size_t block_size;

        // When enabled new space is initialized to 0xaa to help track
        // memory errors in the implementation.
        bool debug_init_aa;

        // The chain of blocks, or 0 if there are none allocated.
        // The most recently allocated block is at the head of the list.
        sv_store_block_t* chain;
} sv_store_region_t;


// ------------------------------------------------------------------------------------------------

// from store/region.c
sv_store_block_t*
sv_store_block_create(
        size_t  size,
        bool    debug_init_aa);

sv_store_region_t*
sv_store_region_create(
        size_t  block_size);

void*
sv_store_region_alloc(
        sv_store_region_t* region,
        size_t bytes);
