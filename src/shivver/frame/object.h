#pragma once
#include "shivver/types.h"
#include "shivver/prim.h"
#include "shivver/frame/tag.h"

typedef uint64_t* frame_t;

// ----------------------------------------------------------------------------
// Application of a primitive.
//  The primitive application frame holds the tag of the primitive being
//  applied, the list of arguments currently being evaluated, and the index
//  of the next argument to evaluate.
//
// PrmF { tag: Word32, len: Word8, ix: Word8, arg: Seq Val }
//
//      7  6  5  4  3   2  1  0
// 0    prim-tag..  len ix 0  F
// 1    arg.ptrs...............
// ..   arg.ptrs...............
//
static inline void
sPrmF   ( uint64_t*     frame
        , uint32_t      tag
        , uint16_t      len
        , uint16_t      ix)
{
        frame[0] = (uint64_t)tag << 32
                 | shivver_prim_tag_args(tag)
                 | (uint64_t)ix  << 16
                 | TAG_PRMF;

        for(size_t i; i < len; i++) {
                frame[i+1] = 0;
        }
}

static inline void
sPrmF_ix(frame_t* frame, uint16_t ix)
{       uint64_t* buf = (uint64_t*)frame;
        buf[0] = (buf[0] & 0xffff00ffffff) | ((uint64_t)ix << 16);

}

static inline void
sPrmF_arg(frame_t* frame, size_t ix, obj_t* obj)
{       uint64_t* buf = (uint64_t*)frame;
        buf[1 + ix] = (uint64_t)obj;
}

static inline uint32_t
xPrmF_tag(frame_t* frame)
{       uint64_t* buf = (uint64_t*)frame;
        return (uint32_t)(buf[0] >> 32);
}


static inline uint16_t
xPrmF_len(frame_t* frame)
{       uint64_t* buf = (uint64_t*)frame;
        return (uint16_t)(buf[0] >> 24);
}


static inline uint16_t
xPrmF_ix(frame_t* frame)
{       uint64_t* buf = (uint64_t*)frame;
        return (uint16_t)(buf[0] >> 16);
}

static inline obj_t*
xPrmF_arg(frame_t* frame, size_t ix)
{       uint64_t* buf = (uint64_t*)frame;
        (obj_t*)buf[1 + ix];
}

