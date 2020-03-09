#pragma once

#include "shivver/runtime.h"

static inline obj_t*
aNatA (uint64_t n)
{       uint64_t* buf = (uint64_t*)halloc(1);
        buf[0]  = ((uint64_t)n << 8) | TAG_NATA;
        return (obj_t*)buf;
}

static inline uint64_t
xNatA (obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return buf[0] >> 8;
}

