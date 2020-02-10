
#include <stdlib.h>
#include <stdio.h>
#include "shivver/runtime.h"


// TODO: string escapes
void
shivver_printP(obj_t* obj)
{
        uint8_t tag = (uint64_t) (obj->header & 0x0ff);

        switch (tag)
        { case TAG_SYMT:
                printf("&symt ");
                break;

          case TAG_VART:
                printf("&vart %zu \"%s\"", xVarT_size(obj), xVarT_name(obj));
                break;

          default:
                printf("&invalid");
                break;
        }
}


// TODO: string escapes.
void
shivver_printL(obj_t* obj)
{
        uint8_t tag = (uint64_t) (obj->header & 0x0ff);

        switch (tag)
        { case TAG_SYMT:
                printf("%%%s", xSymT_name(obj));
                break;

          case TAG_VART:
                printf("%s", xVarT_name(obj));
                break;

          default:
                printf("&invalid");
                break;
        }

}


int main(int argc, char** argv)
{
        shivver_heapInit(1024);
        obj_t* obj = aVarT ("derp", 0);

        shivver_printP(obj);
        printf("\n");

        shivver_printL(obj);
        printf("\n");
}


