#pragma once

// Tags of primitive operators.
//  The 4 byte tag is encoded as:
//
//      bb nn aa rr
//
//  bb: block number                    (eg 10 for nat'* primitives)
//  nn: primitive number in block       (eg 05 for nat'eq)
//  aa: number of arguments             (eg 02 for 2 args for nat'eq)
//  rr: number of results               (eg 01 for 1 results from nat'eq)
//
//  For both the number of arguments and number of results,
//  using the value 'ff' indicates the primitive is polymorphic
//  in the number of arguments and/or results.
//
#define PRIM_NONE       0x00000000

// control
#define PRIM_IF         0x010103ff
#define PRIM_MATCH      0x0102ffff

// nat
#define PRIM_NAT_ADD    0x10010201
#define PRIM_NAT_SUB    0x10020201
#define PRIM_NAT_MUL    0x10030201
#define PRIM_NAT_DIV    0x10040201
#define PRIM_NAT_EQ     0x10050201
#define PRIM_NAT_LT     0x10060201
#define PRIM_NAT_LE     0x10070201
#define PRIM_NAT_GT     0x10080201
#define PRIM_NAT_GE     0x10090201

