
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "sv/store/region.h"
#include "sv/store/rope.h"


// Allocate a new rope node into the given region,
// to join the given two left and right branches.
sv_store_rope_t*
sv_store_rope_join(
        sv_store_region_t* region,
        sv_store_rope_t* left,
        sv_store_rope_t* right)
{
        sv_store_rope_join_t* join
         = sv_store_region_alloc(region,
                sizeof(sv_store_rope_join_t));

        join->tag   = 1;
        join->size  = left->super.size + right->super.size;
        join->left  = left;
        join->right = right;

        return (sv_store_rope_t*)join;
}


// Allocate a new rope leaf node into the given region,
// and copy in data from an existing null terminated string.
sv_store_rope_t*
sv_store_rope_fromString(
        sv_store_region_t* region,
        char* string)
{
        size_t size = strlen(string) + 1;

        sv_store_rope_leaf_t* leaf
         = sv_store_region_alloc(region,
                sizeof(sv_store_rope_leaf_t) + size);

        leaf->tag  = 0;
        leaf->size = size;
        memcpy(leaf->space, string, size);

        return (sv_store_rope_t*)leaf;
}


// Formatted output conversion producing a rope allocated
// into the given region.
sv_store_rope_t*
sv_store_rope_printf(
        sv_store_region_t* region,
        char* format,
        ...)
{
        va_list(args);
        va_start(args, format);
        char* string = 0;
        vasprintf(&string, format, args);
        va_end(args);

        sv_store_rope_t* rope
         = sv_store_rope_fromString(
                region, string);
        free(string);

        return rope;
}


// Concatenate data in a rope into an existing string
//  returning the destination offset of the next character
//  to write into.
static char*
sv_store_rope_toString_flatten(
        sv_store_rope_t* rope,
        char* string)
{
        assert(rope != 0);
        assert(string != 0);

        // leaf node
        if(rope->super.tag == 0) {
                assert(rope->leaf.size >= 1);

                // we copy the whole string, including the null terminator.
                memcpy(string, rope->leaf.space, rope->leaf.size);

                // offset for the next copied chunk.
                return string + rope->leaf.size - 1;
        }
        else if (rope->super.tag == 1) {
                assert(rope->join.size >= 1);

                char* stringLeft
                 = sv_store_rope_toString_flatten(
                        rope->join.left, string);

                char* stringRight
                 = sv_store_rope_toString_flatten(
                        rope->join.right, stringLeft);

                return stringRight;
        }
        else assert(false);
}


// Concatenate data in a rope into a freshly malloced string.
char*
sv_store_rope_toString(
        sv_store_rope_t* rope)
{
        // The allocated string here will tend to be longer than needed
        // because we count the null terminating characters on the end of
        // each chunk, rather than the end of the entire string.
        char* string = malloc(rope->super.size);

        sv_store_rope_toString_flatten(rope, string);
        return string;
}


sv_store_rope_t*
sv_store_rope_string_parens(
        sv_store_region_t* region,
        sv_store_rope_t* inner)
{
        sv_store_rope_t* rope
         = sv_store_rope_fromString(region, "(");

        rope = sv_store_rope_join(region, rope, inner);

        rope = sv_store_rope_join(region, rope,
         sv_store_rope_fromString(region, ")"));

        return rope;
}