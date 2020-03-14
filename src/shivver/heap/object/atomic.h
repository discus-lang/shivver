#pragma once
#include "shivver/heap/alloc.h"


// ----------------------------------------------------------------------------
// Allocate an atomic name object in the heap and copy
// the name string into it.
//
// Atomic names have their characters stored in the object itself.
// The atomic name format is used for var, sym, prm, and mac objects.
//
//      7  6  5  4  3  2  1  0
//  0   len.......  aux....  F
//  1   chars.................
//  ... chars.................
//
//  len: records the length of the string in bytes,
//       not including the trailing null byte.
//
static inline obj_t*
aNameA  ( uint8_t       tag
        , uint24_t      aux
        , uint32_t      len
        , char*         str)
{
        uint64_t len64  = (uint64_t)len;

        // Length of the string including the null byte.
        uint64_t lenn   = len64 + 1;

        // Size of the whole object in 64-bit words.
        uint64_t lenw  = (lenn + 7) << 3;

        // Allocate the object and write the header.
        uint64_t* buf64 = halloc(1 + lenw);
        buf64[0] = ((uint64_t)len << 32)
                 | ((uint64_t)aux << 8)
                 | (uint64_t)tag;

        // Copy in the characters and null byte.
        uint8_t*  buf8  = (uint8_t*)buf64;
        for (size_t i = 0; i < len; i++)
        {       buf8[8 + i] = str[i];
        }
        buf8[8 + len] = 0;
        return (obj_t*)buf8;
}

static inline uint32_t
xNameA_len(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}

static inline uint24_t
xNameA_aux(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)((buf[0] >> 8) & 0x0ffffff);
}

static inline char*
xNameA_str(obj_t* obj)
{       uint8_t* buf = (uint8_t*)obj;
        return (char*)(buf + 8);
}



// ----------------------------------------------------------------------------
// A variable name with the characters stored in the object.
//
//      7  6  5  4  3  2  1  0
//  0   len.......  bump...  F
//  1   chars.................
//  ... chars.................
//
static inline obj_t*
aVarA (uint32_t len, char* str)
{       return aNameA(TAG_VARA, 0, len, str); }

static inline uint32_t
xVarA_len(obj_t* obj)
{       return xNameA_len(obj); }

static inline char*
xVarA_name(obj_t* obj)
{       return xNameA_str(obj); }

static inline uint24_t
xVarA_bump(obj_t* obj)
{       return xNameA_aux(obj); }


// ----------------------------------------------------------------------------
// A symbol name with the characters stored in the object.
//
//      7  6  5  4  3  2  1  0
//  0   len.......  0  0  0  F
//  1   chars.................
//  ... chars.................
//
static inline obj_t*
aSymA (uint32_t len, char* str)
{       return aNameA(TAG_SYMA, 0, len, str); }

static inline uint32_t
xSymA_len(obj_t* obj)
{       return xNameA_len(obj); }

static inline char*
xSymA_name (obj_t* obj)
{       return xNameA_str(obj); }


// ----------------------------------------------------------------------------
// A primitive name with the characters stored in the object.
//
//      7  6  5  4  3  2  1  0
//  0   len.......  0  0  0  F
//  1   chars.................
//  ..  chars.................
//
static inline obj_t*
aPrmA (uint32_t len, char* str)
{       return aNameA(TAG_PRMA, 0, len, str); }

static inline uint32_t
xPrmA_len(obj_t* obj)
{       return xNameA_len(obj); }

static inline char*
xPrmA_name(obj_t* obj)
{       return xNameA_str(obj); }


// ----------------------------------------------------------------------------
// A primitive operator tag.
//
//      7  6  5  4  3  2  1  0
//  0   prim-tag..  0  0  0  F
//
static inline obj_t*
aPrzA (uint32_t tag)
{       uint64_t* buf   = halloc(1);
        buf[0]  = (((uint64_t)tag) << 32) | TAG_PRZA;
        return (obj_t*)buf;
}

static inline uint32_t
xPrzA_tag(obj_t* obj)
{       uint64_t* buf = (uint64_t*)obj;
        return (uint32_t)(buf[0] >> 32);
}


// ----------------------------------------------------------------------------
// A macro name with the characters stored in the object.
//
//      7  6  5  4  3  2  1  0
//  0   len.......  0  0  0  F
//  1   chars.................
//  ..  chars.................
//
static inline obj_t*
aMacA (uint32_t len, char* str)
{       return aNameA(TAG_MACA, 0, len, str); }

static inline uint32_t
xMacA_len(obj_t* obj)
{       return xNameA_len(obj); }

static inline char*
xMacA_name(obj_t* obj)
{       return xNameA_str(obj); }



// ----------------------------------------------------------------------------
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

