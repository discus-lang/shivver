#include "shivver/util.h"


// Allocate a new object list.
objlist_t*
shivver_objlist_alloc()
{
        objlist_t* list = (objlist_t*)malloc(sizeof(objlist_t));
        list->list      = (obj_t**)malloc(sizeof(obj_t*) * 16);
        list->len       = 16;
        list->used      = 0;

        return list;
}


// Free an object list.
void
shivver_objlist_free(objlist_t* list)
{
        free(list->list);
        free(list);
}


// Append an object onto the end of an object list.
void
shivver_objlist_append(objlist_t* list, obj_t* obj)
{
        if (list->used == list->len)
        {
                size_t newLen
                 = list->len * 2;

                obj_t** newList
                 = (obj_t**)malloc(sizeof(obj_t*) * newLen);

                memcpy(newList, list->list, sizeof(obj_t) * list->len);

                list->list = newList;
                list->len  = newLen;
        }

        list->list[list->used] = obj;
        list->used++;
}




