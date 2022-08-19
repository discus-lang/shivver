#pragma once
#include <stdlib.h>
#include "sv/store/region.h"


/* --------------------------------------------------------------------------------------------- */

union sv_store_rope_t_;

// Supertype of rope nodes.
//  A rope is a tree structure where data is stored in the leaves,
//  and join nodes are used to construct compound ropes so that the
//  contained data can be read out sequentially.
typedef struct {
        // Tag indicating leaf(0) or join(1);
        size_t tag;

        // Total number of bytes stored in this node.
        size_t size;
} sv_store_rope_super_t;


// Rope nodes
typedef struct {
        // Tag indicating leaf(0) or join(1);
        size_t tag;

        // Total number of bytes stored in this node.
        size_t size;

        // Payload data.
        uint8_t space[];
} sv_store_rope_leaf_t;


// Rope node joining two smaller ropes.
typedef struct {
        // Tag indicating leaf(0) or join(1);
        size_t tag;

        // Total number of bytes stored in the rope.
        size_t size;

        // Node of left branch.
        union sv_store_rope_t_* left;

        // Node of right branch.
        union sv_store_rope_t_* right;
} sv_store_rope_join_t;


typedef union sv_store_rope_t_ {
        sv_store_rope_super_t super;
        sv_store_rope_leaf_t  leaf;
        sv_store_rope_join_t  join;
} sv_store_rope_t;


/* --------------------------------------------------------------------------------------------- */

sv_store_rope_t*
sv_store_rope_join(
        sv_store_region_t* region,
        sv_store_rope_t* left,
        sv_store_rope_t* right);

sv_store_rope_t*
sv_store_rope_fromString(
        sv_store_region_t* region,
        char* string);

sv_store_rope_t*
sv_store_rope_printf(
        sv_store_region_t* region,
        char* format,
        ...);

char*
sv_store_rope_toString(
        sv_store_rope_t* rope);

sv_store_rope_t*
sv_store_rope_string_parens(
        sv_store_region_t* region,
        sv_store_rope_t* inner);
