#pragma once
#include "shivver/runtime/heap.h"


// Generic functions that work for all object types.

// Get the tag from an object header.
static inline uint8_t
xObj_tag (obj_t* obj)
{       return (uint8_t)((uint64_t) (obj->header & 0x0ff));
}
