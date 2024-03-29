#pragma once
#include "shivver/heap/alloc.h"

// ----------------------------------------------------------------------------
// A vector of terms.
//  physical: (&mmmh N Term_0 ... Term_N)
//  logical:  [Term_0 ... Term_N]
//
//      7  6  5  4  3  2  1  0
//  0   len.......  0  0  0  F
//  1   pointer 0.............
//  ... pointer...............
//
static inline obj_t*
aMmmH (uint32_t len32, obj_t** arg)
{
        uint64_t  len = (uint64_t)len32;
        uint64_t* buf = halloc(1 + len);
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
// An abstraction, with parameters represented as variable names in the heap.
//
//      7  6  5  4  3  2  1  0
//  0   len.......  0  0  0  F
//  1   body pointer
//  2   variable pointer 0 ...
//  ... variable pointer .....
//
static inline obj_t*
aAbsH (uint32_t len, obj_t* oParm[], obj_t* oBody)
{       uint64_t* buf = halloc(2 + len);
        buf[0] = (uint64_t)len << 32 | TAG_ABSH;
        buf[1] = (uint64_t)oBody;
        for (size_t i = 0; i < len; i++)
                buf[2 + i] = (uint64_t)oParm[i];
        return (obj_t*)buf;
}

// Get the number of parameter variables.
static inline uint32_t
xAbsH_len (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

// Get a pointer to the array of variable object pointers,
// which is inside the object.
static inline obj_t**
xAbsH_parmp (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t**)(buf + 2);
}

// Get the pointer to the ith variable object pointer.
static inline obj_t*
xAbsH_parm (obj_t* obj, size_t i)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[2 + i];
}

// Get the pointer to the body term.
static inline obj_t*
xAbsH_body (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}


// ----------------------------------------------------------------------------
// An application to a high arity term.
//
//      7  6  5  4  3  2  1  0
//  0   0  0  0  0  0  0  0  F
//  1   head pointer
//  2   args pointer
//
static inline obj_t*
aAppH (obj_t* oFun, obj_t* oArg)
{       uint64_t* buf = halloc(3);
        buf[0] = TAG_APPH;
        buf[1] = (uint64_t)oFun;
        buf[2] = (uint64_t)oArg;
        return (obj_t*)buf;
}

static inline obj_t*
xAppH_fun (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}

static inline obj_t*
xAppH_arg (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[2];
}


// ----------------------------------------------------------------------------
// An application to a vector of terms.
//  physical: (&apsh N TermHead TermArg_0 ... TermArg_N)
//  logical:  (TermHead [TermArg_0, ..., TermArg_N])
//
//      7  6  5  4  3  2  1  0
//  0   len.......  0  0  0  F
//  1   head pointer
//  2   arg pointer 0
//  ... arg pointer ...
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
// A closure, with parameters represented as symbols in the heap.
//
//      7  6  5  4  3  2  1  0
//  0   len.......  0  0  0  F
//  1   env pointer             obj_t*
//  2   symbols pointer         obj_t**  always points into an xAbsH.
//  3   body pointer            obj_t*
//
static inline obj_t*
aCloH (uint32_t len, obj_t* oEnv, obj_t** oParm, obj_t* oBody)
{       uint64_t* buf = halloc(4);
        buf[0] = (uint64_t)len << 32 | TAG_CLOH;
        buf[1] = (uint64_t)oEnv;
        buf[2] = (uint64_t)oParm;
        buf[3] = (uint64_t)oBody;
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
xCloH_parmp (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t**)buf[2];
}

static inline obj_t*
xCloH_body (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[3];
}


// ----------------------------------------------------------------------------
// An environment frame, with parameters represented as symbols in the heap.
//
//      7  6  5  4  3  2  1  0
//  0   len.......  0  0  0  F
//  1   parent pointer          obj_t*
//  2   variables pointer       obj_t**  always points into an xAbsH.
//  3   values pointer          obj_t*
//  ... values pointer...
//
static inline obj_t*
aEnvH (uint32_t len, obj_t* oEnv, obj_t** oParms, obj_t* oArgs[])
{
        uint64_t* buf = halloc(3 + len);
        buf[0] = (uint64_t)len << 32 | TAG_ENVH;
        buf[1] = (uint64_t)oEnv;
        buf[2] = (uint64_t)oParms;

        for (size_t i = 0; i < len; i++)
                buf[3 + i] = (uint64_t)oArgs[i];

        return (obj_t*)buf;
}

static inline uint32_t
xEnvH_len (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

static inline obj_t*
xEnvH_parent (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)buf[1];
}

static inline obj_t**
xEnvH_varp (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t**)buf[2];
}

static inline obj_t*
xEnvH_var (obj_t* obj, uint32_t i)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)((obj_t**)buf[2])[i];
}


static inline obj_t*
xEnvH_val (obj_t* obj, uint32_t i)
{       uint64_t* buf = (uint64_t*)obj;
        return (obj_t*)(buf[3 + i]);
}

