
#include <assert.h>
#include "sv/source.h"
#include "sv/store.h"

int main(int argc, char* argv[])
{
        size_t block_size = 4096;
        sv_store_region_t* region
         = sv_store_region_create(block_size);

        sv_source_parse_t* state
         = sv_source_parse_alloc("derp");

        sv_source_term_t* term
         = sv_source_parse_term(
                state, region);

        assert(term != 0);
}