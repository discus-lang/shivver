#pragma once
#include "shivver/types.h"

// Objects
//                                      sort    mode
//         7  6  5  4  3  2  1  0       ------- ---

//  mmmh   len.ptrs... 0  0  0  F       0 0 0 0 1 1 0 1  = 0x0d
//  absh   len.ptrs... 0  0  0  F       0 1 0 0 1 1 0 1  = 0x4d
//  apvh   0  0  0  0  0  0  0  F       0 1 0 1 1 1 0 1  = 0x5d
//  apsh   len.ptrs... 0  0  0  F       0 1 1 0 1 1 0 1  = 0x6d
//  cloh   len.ptrs... 0  0  0  F       1 0 0 0 1 1 0 1  = 0x8d
//  envh   len.ptrs... 0  0  0  F       1 0 0 1 1 1 0 1  = 0x9d

//  vart   0  0  0  0  bump  .  F       0 0 0 0 0 1 0 1  = 0x05
//  symt   0  0  0  0  bump  .  F       0 0 0 1 0 1 0 1  = 0x15
//  prmt   0  0  0  0  bump  .  F       0 0 1 0 0 1 0 1  = 0x25

//  vara   len.chars.. 0  0  0  F       0 0 0 0 0 0 0 1  = 0x01
//  syma   len.chars.. 0  0  0  F       0 0 0 1 0 0 0 1  = 0x11
//  prma   len.chars.. 0  0  0  F       0 0 1 0 0 0 0 1  = 0x21
//  maca   len.chars.. 0  0  0  F       0 0 1 0 0 0 0 1  = 0x31

//  prza   prm-tag.... 0  0  0  F       0 0 1 0 0 0 0 1  = 0x41

//  blla   value..............  F       0 0 0 0 0 0 1 1  = 0x03
//  nata   value..............  F       0 0 0 1 0 0 1 1  = 0x13

//  mode: 00 atomic
//        01 static
//        10 cold
//        11 hot
//
//  sort: 0000 mmm
//        0100 abs
//        0101 app
//        0110 aps
//        1000 clo
//        1001 env
//
//  lit:  0000 bll
//        0001 nat
//

#define TAG_MMMH        0x0d
#define TAG_ABSH        0x4d
#define TAG_APPH        0x5d
#define TAG_APSH        0x6d
#define TAG_CLOH        0x8d
#define TAG_ENVH        0x9d

#define TAG_VART        0x05
#define TAG_SYMT        0x15
#define TAG_PRMT        0x25

#define TAG_VARA        0x01
#define TAG_SYMA        0x11
#define TAG_PRMA        0x21
#define TAG_MACA        0x31

#define TAG_PRZA        0x41

#define TAG_BLLA        0x03
#define TAG_NATA        0x13

