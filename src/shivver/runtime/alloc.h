#pragma once
#include <string.h>
#include "shivver/runtime.h"

static inline uint8_t
xObj_tag (obj_t* obj)
{       return (uint8_t)((uint64_t) (obj->header & 0x0ff));
}

// --
// For cold storage
//  running program will be "hot" and use real pointers.
//  cold storage must always use handles.


// ----------------------------------------------------------------------------
// A vector of terms.
//
//     7 6 5 4 3 2 1 0
// 0   size    0 . . F
// 1   pointer 0
// 2.. pointer 1..
//
static inline obj_t*
aMmmH (size_t len, obj_t** arg)
{       uint64_t* buf = halloc(1 + len);
        buf[0] = len << 32 | TAG_MMMH;
        for (size_t i = 0; i < len; i++)
                buf[1 + i] = (uint64_t)arg[i];
        return (obj_t*)buf;
}


static inline uint32_t
xMmmH_len(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

static inline obj_t*
xMmmH_arg(obj_t* obj, size_t i)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1 + i];
}

static inline obj_t**
xMmmH_args(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t**)(buf + 1);
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
aApsH (uint32_t len, obj_t* oFun, obj_t* oArg[])
{       uint64_t* buf = halloc(2 + len);
        buf[0] = (uint64_t)len << 32 | TAG_APSH;
        buf[1] = (uint64_t)oFun;
        for (size_t i = 0; i < len; i++)
                buf[2 + i] = (uint64_t)oArg[i];
        return (obj_t*)buf;
}

static inline uint32_t
xApsH_len (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

static inline obj_t*
xApsH_fun (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}

static inline obj_t*
xApsH_arg (obj_t* obj, size_t i)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[2 + i];
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
aApvH (obj_t* oFun, obj_t* oArg)
{       uint64_t* buf = halloc(3);
        buf[0] = TAG_APVH;
        buf[1] = (uint64_t)oFun;
        buf[2] = (uint64_t)oArg;
        return (obj_t*)buf;
}

static inline obj_t*
xApvH_fun (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}

static inline obj_t*
xApvH_arg (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[2];
}


// ----------------------------------------------------------------------------
// An abstraction, with parameters represented as symbols in the heap.
//
//    7 6 5 4 3 2 1 0
// 0  count . 0 0 0 F
// 1  body pointer
// 2  symbol pointer
// 3  symbol pointer..
//
static inline obj_t*
aAbsH (uint32_t count, obj_t* oParm[], obj_t* oBody)
{       uint64_t* buf = halloc(2 + count);
        buf[0] = (uint64_t)count << 32 | TAG_ABSH;
        buf[1] = (uint64_t)oBody;
        for (size_t i = 0; i < count; i++)
                buf[2 + i] = (uint64_t)oParm[i];
        return (obj_t*)buf;
}

static inline uint32_t
xAbsH_len (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

static inline obj_t*
xAbsH_parm (obj_t* obj, size_t i)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[2 + i];
}

static inline obj_t**
xAbsH_parms (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t**)(buf + 2);
}

static inline obj_t*
xAbsH_body (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}


// ----------------------------------------------------------------------------
// A closure, with parameters represented as symbols in the heap.
//
//    7 6 5 4 3 2 1 0
// 0  count . 0 0 0 F
// 1  env pointer       obj_t*
// 2  symbols pointer   obj_t**  always points into an xAbsM.
// 3  body pointer      obj_t*
//
static inline obj_t*
aCloH (uint32_t len, obj_t* oEnv, obj_t** oParm, obj_t* oBody)
{       uint64_t* buf = halloc(3);
        buf[0] = (uint64_t)len << 32 | TAG_CLOH;
        buf[1] = (uint64_t)oEnv;
        buf[2] = (uint64_t)oParm;
        buf[2] = (uint64_t)oBody;
        return (obj_t*)buf;
}

static inline uint32_t
xCloH_len (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

static inline obj_t*
xCloH_env (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}

static inline obj_t**
xCloH_parms (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t**)buf[2];
}

static inline obj_t*
xCloH_body (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[2];
}


// ----------------------------------------------------------------------------
// An environment frame, with parameters represented as symbols in the heap.
//
//    7 6 5 4 3 2 1 0
// 0  count . 0 0 0 F
// 1  parent pointer   obj_t*
// 2  symbols pointer  obj_t**  always points into an xAbsM.
// 3  values  pointer  obj_t**  always points into an xMmm.
//
static inline obj_t*
aEnvH (uint32_t count, obj_t* oEnv, obj_t** oParms, obj_t** oArgs)
{       uint64_t* buf = halloc(4);
        buf[0] = (uint64_t)count << 32 | TAG_ENVH;
        buf[1] = (uint64_t)oEnv;
        buf[2] = (uint64_t)oParms;
        buf[3] = (uint64_t)oArgs;
        return (obj_t*)buf;
}

