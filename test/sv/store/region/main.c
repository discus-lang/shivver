
#include <assert.h>
#include "sv/store.h"


int main(int argc, char* argv[])
{
        // simple allocation.
 {      sv_store_region_t* region
         = sv_store_region_create(256);

        void* buf = sv_store_region_alloc(region, 16);
        assert(buf != 0);
        assert(region->count_blocks == 1);
        assert(region->count_space_allocated == 16);
        sv_store_region_free(region);
 }

        // allocate all the space available in a single block.
{       sv_store_region_t* region
         = sv_store_region_create(256);

        void* buf = sv_store_region_alloc(region, 256);
        assert(buf != 0);
        assert(region->count_blocks == 1);
        assert(region->count_space_allocated == 256);
        sv_store_region_free(region);
}

        // force allocation of three separate blocks.
 {      sv_store_region_t* region
         = sv_store_region_create(3 * 8);

        sv_store_region_alloc(region, 2*8);
        sv_store_region_alloc(region, 2*8);
        sv_store_region_alloc(region, 2*8);
        assert(region->count_blocks == 3);
        assert(region->count_space_allocated == 6 * 8);
        sv_store_region_free(region);
 }

}