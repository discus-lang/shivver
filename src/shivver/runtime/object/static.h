#pragma once

// Objects that refer to data in statically allocated memory.
//  either 1) the text segment of the unix process, or
//         2) memory that was allocated at startup and lives forever.
#include "shivver/runtime.h"


// ----------------------------------------------------------------------------
// A symbol with a statically allocated name string.
//
//     7 6 5 4 3 2 1 0
// 0   len . . 0 0 0 F
// 1   ptr to c-string
static inline obj_t*
aSymT (const char* name)
{       size_t   len  = strlen(name);
        uint64_t* buf = halloc(2);
        buf[0] = len << 32 | TAG_SYMT;
        buf[1] = (uint64_t)name;
        return (obj_t*)buf;
}

static inline uint32_t
xSymT_size(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return buf[0] >> 32;
}

static inline char*
xSymT_name(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (char*)buf[1];
}


// ----------------------------------------------------------------------------
// A variable with a statically allocated name string.
//
//     7 6 5 4 3 2 1 0
// 0   len . . bump  F
// 1   ptr to c-string
//
static inline obj_t*
aVarT (const char* name, uint24_t nBump)
{       size_t    len = strlen(name);
        uint64_t* buf = halloc(2);
        buf[0] = len << 32 | nBump << 8 | TAG_VART;
        buf[1] = (uint64_t)name;
        return (obj_t*)buf;
}

static inline size_t
xVarT_size(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (size_t)buf[0] >> 32;
}

static inline char*
xVarT_name(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (char*)buf[1];
}

static inline uint24_t
xVarT_bump(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint24_t)(buf[0] >> 8) & 0x0ffffff;
}
