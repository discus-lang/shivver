#pragma once
#include <stdlib.h>
#include <stdint.h>


// A slab of contiguous space.
typedef struct {
        // Size of the usable space, in bytes.
        size_t  size;

        // Index of the next byte to allocate.
        size_t  next;

        // Storage space in the slab.
        uint8_t space;
} sv_store_slab_t;