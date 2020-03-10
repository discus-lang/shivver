#pragma once

// Tags of primitive operators.
//  The 4 byte tag is encoded as:
//
//      bb nn aa rr
//
//  bb: block number                    (eg 01 for nat'* primitives)
//  nn: primitive number in block       (eg 05 for nat'eq)
//  aa: number of arguments             (eg 02 for 2 args for nat'eq)
//  rr: number of results               (eg 01 for 1 results from nat'eq)
//
#define PRIM_NAT_ADD    0x01010201
#define PRIM_NAT_SUB    0x01020201
#define PRIM_NAT_MUL    0x01030201
#define PRIM_NAT_DIV    0x01040201
#define PRIM_NAT_EQ     0x01050201
#define PRIM_NAT_LT     0x01060201
#define PRIM_NAT_LE     0x01070201
#define PRIM_NAT_GT     0x01080201
#define PRIM_NAT_GE     0x01090201

#include "shivver/prim/console.h"


// Get the name of the primitive wiht the given tag.
char*
shivver_prim_tag_name
        (uint32_t tag);


// Get the primitive block number from a tag.
static inline size_t
shivver_prim_tag_block
        (uint32_t tag)
{       return (tag >> 24) & 0xff;
}


// Get the primitive number with the block from a tag.
static inline size_t
shivver_prim_tag_prim
        (uint32_t tag)
{       return (tag >> 16) & 0xff;
}


// Get the number of primitive arguments from a tag.
static inline size_t
shivver_prim_tag_args
        (uint32_t tag)
{       return (tag >> 8) & 0xff;
}


// Get the number of primitive results from a tag.
static inline size_t
shivver_prim_tag_results
        (uint32_t tag)
{       return tag & 0xff;
}

