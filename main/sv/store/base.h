#pragma once
#include <stdint.h>

// bits in object flags
#define SV_STORE_BITS_FLAGS             8

// flags bit mask for hot/cold flag.
#define SV_STORE_FLAG_HOT_COLD          0x80

// bits in object tag
#define SV_STORE_BITS_TAG               8

// bits in name definition length
//  limits the number of bytes in a name.
#define SV_STORE_BITS_NAME_LENGTH       16

// bits in cold region index
//  limits the number of cold regions in a zone.
#define SV_STORE_BITS_COLD_REGION       24

// bits in argument count.
//  limits the length of an argument list.
#define SV_STORE_BITS_MMM_COUNT         24

// bit mask to select store ref tag bits.
//  All objects in the hot space are 64-bit aligned,
//  so the lowest 3 bits in byte references are guaranteed to be zero.
#define SV_STORE_MASK_REF_LOW           0x07


// A pointer to a hot space object.
typedef uint64_t sv_store_ptr_t;

// An offset to a cold space object,
//  relative to the start of the slab containing the enclosing object.
typedef uint64_t sv_store_off_t;


// An object reference.
//  References to hot space objects are via machine pointers.
//  References to cold space objects are via offsets from the
//  start of the containing slab.
typedef union  {
        uint64_t raw;
        sv_store_ptr_t ptr;
        sv_store_off_t off;
} sv_store_ref_t;



