#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ------------------------------------------------------------------------------------------------
// A block of storage space including the size and offset for the
// next allocation.
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
//  region then free all objects together when they are no longer needed.
typedef struct {
        // Block size, in bytes.
        //  This is the size of the usable space,
        //  not including the block header.
        size_t block_size;

        // The chain of blocks, or 0 if there are none allocated.
        // The most recently allocated block is at the head of the list.
        sv_store_block_t* block_chain;

        // When enabled new space is initialized to 0xaa to help track
        // memory errors in the implementation.
        bool debug_init_aa;

        // Number of blocks currently in the chain.
        size_t count_blocks;

        // Amount of allocated space in the region.
        size_t count_space_allocated;
} sv_store_region_t;


// ------------------------------------------------------------------------------------------------
// from store/region.c
sv_store_region_t*
sv_store_region_create(
        size_t  block_size);

void*
sv_store_region_alloc(
        sv_store_region_t* region,
        size_t bytes);

sv_store_block_t*
sv_store_block_create(
        size_t  size,
        bool    debug_init_aa);
