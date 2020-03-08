#pragma once
#include "shivver/runtime.h"

// ------------------------------------------------------------------------------------------------
typedef struct
{       obj_t** list;   // array of object pointers.
        size_t  len;    // length of the array.
        size_t  used;   // number of slots already used in the array.
} objlist_t;


objlist_t*
shivver_objlist_alloc();

void
shivver_objlist_free(objlist_t* objlist);

void
shivver_objlist_append(objlist_t* list, obj_t* obj);

