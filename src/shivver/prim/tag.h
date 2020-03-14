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