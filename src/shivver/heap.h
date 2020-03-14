#pragma once
#include "shivver/types.h"
#include "shivver/heap/types.h"
#include "shivver/heap/tag.h"
#include "shivver/heap/object/base.h"
#include "shivver/heap/object/atomic.h"
#include "shivver/heap/object/static.h"
#include "shivver/heap/object/hot.h"
#include "shivver/error.h"


// from heap/init.c
void    shivver_heapInit(size_t nWords);
