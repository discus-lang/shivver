
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "sv/store.h"

// Create a new store block of the given size in bytes.
sv_store_block_t*
sv_store_block_create(
        size_t  size,
        bool    debug_init_aa)
{
        assert(size >= 1);

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


// Create a new empty region with no blocks.
sv_store_region_t*
sv_store_region_create(
        size_t  block_size)
{
        assert(block_size >= 1);

        sv_store_region_t* region
         = malloc(sizeof(sv_store_region_t));
        memset(region, 0, sizeof(sv_store_region_t));

        region->block_size = block_size;
        region->debug_init_aa = true;
        region->chain = 0;

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

        // We don't yet support allocating space more than a block.
        // In future we could store a list of large objects.
        assert(bytes <= region->block_size);

        // First block in the chain, or 0 if none have been created yet.
        sv_store_block_t* block
         = region->chain;

        // Chain contains no blocks, so create the first one.
        if(block == 0) {
                block
                 = sv_store_block_create(
                        region->block_size,
                        region->debug_init_aa);

                region->chain = block;
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

                block->link = region->chain;
                region->chain = block;
        }

        // At this point there is guaranteed to be enough space
        // in the block.
        assert((block->size - block->next) >= bytes);

        void* buf = block->space;
        block->next += bytes;

        // Next pointer is either less then the size,
        // or exactly equal to it which indicates a fully allocated block.
        assert(block->next <= block->size);

        return buf;
}


