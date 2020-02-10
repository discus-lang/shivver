#pragma once
#include <string.h>
#include "shivver/runtime.h"


// ----------------------------------------------------------------------------
// A vector of terms.
//
//     7 6 5 4 3 2 1 0
// 0   size    0 . . F
// 1   pointer 0
// 2.. pointer 1..
//
static inline obj_t*
aMmm (size_t len, obj_t** arg)
{       uint64_t* buf = halloc(1 + len);
        buf[0] = len << 32 | TAG_MMM;
        for (size_t i = 0; i < len; i++)
                buf[1 + i] = (uint64_t)arg[i];
        return (obj_t*)buf;
}


static inline uint32_t
xMmm_size(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

static inline obj_t*
xMmm_arg(obj_t* obj, size_t i)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1 + i];
}

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


// ----------------------------------------------------------------------------
// An application to a vector of terms.
//
//     7  6  5  4  3  2  1  0
// 0   count .  .  0  0  0  F
// 1   head pointer
// 2   arg pointer 1
// 3.. arg pointer ...
//
static inline obj_t*
aAppS (uint32_t len, obj_t* oFun, obj_t* oArg[])
{       uint64_t* buf = halloc(2 + len);
        buf[0] = (uint64_t)len << 32 | TAG_APPS;
        buf[1] = (uint64_t)oFun;
        for (size_t i = 0; i < len; i++)
                buf[2 + i] = (uint64_t)oArg[i];
        return (obj_t*)buf;
}

static inline uint32_t
xAppS_len (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

static inline obj_t*
xAppS_fun (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}

static inline obj_t*
xAppS_arg (obj_t* obj, size_t n)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[2 + n];
}


// ----------------------------------------------------------------------------
// An application to a high-arity term.
//
//    7 6 5 4 3 2 1 0
// 0  0 0 0 0 0 0 0 F
// 1  head pointer
// 2  args pointer
//
static inline obj_t*
aAppV (obj_t* oFun, obj_t* oArg)
{       uint64_t* buf = halloc(3);
        buf[0] = TAG_APPV;
        buf[1] = (uint64_t)oFun;
        buf[2] = (uint64_t)oArg;
        return (obj_t*)buf;
}

static inline obj_t*
xAppV_fun (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}

static inline obj_t*
xAppV_arg (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[2];
}
