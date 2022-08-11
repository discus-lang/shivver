
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "sv/store.h"


// Create a new empty region with no blocks.
sv_store_region_t*
sv_store_region_create(
        size_t  block_size)
{
        // Prevent misaligned allocations.
        assert(block_size >= 1);
        assert(block_size % 8 == 0);

        // Create the new region.
        sv_store_region_t* region
         = malloc(sizeof(sv_store_region_t));
        memset(region, 0, sizeof(sv_store_region_t));

        region->block_size      = block_size;
        region->block_chain     = 0;
        region->debug_init_aa   = true;

        return region;
}


// Allocate space in a region.
void*
sv_store_region_alloc(
        sv_store_region_t* region,
        size_t bytes)
{
        assert(region != 0);
        assert(bytes >= 1);

        // Prevent misaligned allocations by increasing the
        // allocation size to get whole 64-bit words.
        size_t tail = bytes % 8;
        if (tail != 0) {
                bytes += (8 - tail);
        }
        assert(bytes % 8 == 0);

        // We don't yet support allocating space more than a block.
        // In future we could store a list of large objects.
        assert(bytes <= region->block_size);

        // First block in the chain, or 0 if none have been created yet.
        sv_store_block_t* block
         = region->block_chain;

        // Chain contains no blocks, so create the first one.
        if(block == 0) {
                block
                 = sv_store_block_create(
                        region->block_size,
                        region->debug_init_aa);

                region->block_chain = block;
                region->count_blocks++;
        }

        // First block does not have enough free space, so create
        // a new one and push it on the chain.
        //
        // As we require the number of allocated bytes to be less
        // than or equal to the block size, the object is
        // guaranteed to fit in this new block.
        else if((block->size - block->next) <= bytes) {
                block
                 = sv_store_block_create(
                        region->block_size,
                        region->debug_init_aa);

                block->link = region->block_chain;
                region->block_chain = block;
                region->count_blocks++;
        }

        // At this point there is guaranteed to be enough space
        // in the block.
        assert((block->size - block->next) >= bytes);

        void* buf = block->space + block->next;
        block->next += bytes;
        region->count_space_allocated += bytes;

        // Next pointer is either less then the size,
        // or exactly equal to it which indicates a fully allocated block.
        assert(block->next <= block->size);

        return buf;
}


// Free a region including all its contained blocks.
void
sv_store_region_free(
        sv_store_region_t* region)
{
        sv_store_block_t* block
         = region->block_chain;

        // Free the chain of blocks.
        for(;;) {
                if (block == 0) break;

                // copy the next block link before we free
                // the one in the current block.
                sv_store_block_t* next_block
                 = block->link;

                // scrub the block header to help reveal latent use-after-free
                // problems earlier.
                block->size = 0;
                block->next = 0;
                block->link = 0;

                free(block);
                block = next_block;
        }

        // scrub the region header to help reveal latent use-after-free
        // problems earlier.
        region->block_chain = 0;
        region->block_size  = 0;
        region->count_blocks = 0;
        region->count_space_allocated = 0;

        // release the top level region structure.
        free(region);
}



// Create a new store block of the given size in bytes.
sv_store_block_t*
sv_store_block_create(
        size_t  size,
        bool    debug_init_aa)
{
        // Prevent misaligned allocations.
        assert(size >= 1);
        assert(size % 8 == 0);

        sv_store_block_t* block
         = malloc(sizeof(sv_store_block_t) + size);

        // Init the space by 0xaa by default to help track memory errors.
        // This can be disabled once the store system is stable.
        if (debug_init_aa) {
                memset(block, 0xaa, sizeof(sv_store_block_t) + size);
        }

        block->size = size;
        block->next = 0;
        block->link  = 0;

        return block;
}

