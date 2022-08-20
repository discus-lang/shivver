#pragma once
#include "sv/store/base.h"
#include "sv/store/region.h"
#include "sv/store/object.h"

// Store zone for a single thread.
//  A store zone contains a closed program graph.
//  The objects in the graph may be located in the hot or cold spaces.
//  Each hot or cold space consists of one or more regions.
//
//  The hot space can include mutable objects.
//   References to hot objects are via machine pointers.
//   References from hot objects are either machine pointers or cold space offsets.
//
//  The cold space is immutable and closed.
//   References to cold objects are via offsets from the start of the space.
//   There are no references from the cold space to the hot space.
//   References from objects in a particular region are only to other objects in the same region.
//
typedef struct {
        // Hot space for newly allocated objects.
        sv_store_region_t* new;

        // Number of regions of cold objects.
        size_t cold_count;

        // Array of regions containing cold objects.
        //  Region 0: reserved for the goal term of the thread.
        //  Region 1: reserved for the name chain used by the hot space.
        //  The other regions may be memory mapped from files,
        //  or share address space with other threads.
        sv_store_region_t** cold;

        // Offset of the head of the name definition chain in cold region 1.
        unsigned int names_head : SV_STORE_BITS_OFFSET;
} sv_store_zone_t;

