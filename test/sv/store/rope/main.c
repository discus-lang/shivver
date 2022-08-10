
#include <assert.h>
#include <stdio.h>
#include "sv/store/region.h"
#include "sv/store/rope.h"

int main(int argc, char* argv[])
{
        sv_store_region_t* region
         = sv_store_region_create(256);

        // single leaf
 {      sv_store_rope_t* rope  = sv_store_rope_fromString(region, "derp");

        char* string = sv_store_rope_toString(rope);

        printf("%s\n", string);
        free(string);
 }

        // single join
 {      sv_store_rope_t* rope1 = sv_store_rope_fromString(region, "one|");
        sv_store_rope_t* rope2 = sv_store_rope_fromString(region, "two|");
        sv_store_rope_t* rope3 = sv_store_rope_join(region, rope1, rope2);

        char* string = sv_store_rope_toString(rope3);

        printf("%s\n", string);
        free(string);
 }

        // multi joins right heavy
 {      sv_store_rope_t* rope1 = sv_store_rope_fromString(region, "one|");
        sv_store_rope_t* rope2 = sv_store_rope_fromString(region, "two|");
        sv_store_rope_t* rope3 = sv_store_rope_fromString(region, "three|");
        sv_store_rope_t* ropeJ
         = sv_store_rope_join(region,
                rope1,
                sv_store_rope_join(region, rope2, rope3));

        char* string = sv_store_rope_toString(ropeJ);
        printf("%s\n", string);
        free(string);
 }

        // multi joins left heavy
 {      sv_store_rope_t* rope1 = sv_store_rope_fromString(region, "one|");
        sv_store_rope_t* rope2 = sv_store_rope_fromString(region, "two|");
        sv_store_rope_t* rope3 = sv_store_rope_fromString(region, "three|");
        sv_store_rope_t* ropeJ
         = sv_store_rope_join(region,
                sv_store_rope_join(region, rope1, rope2),
                rope3);

        char* string = sv_store_rope_toString(ropeJ);
        printf("%s\n", string);
        free(string);
 }

}