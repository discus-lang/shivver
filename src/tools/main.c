
#include <stdlib.h>
#include <stdio.h>
#include "shivver/runtime.h"




int main(int argc, char** argv)
{
        shivver_heapInit(1024);
        obj_t* obj
         = aAppV( aVarT ("derp", 0)
                , aMmm (2, (obj_t*[]){aSymT("fresh"), aSymT("fish")}));

        shivver_printp(obj);
        printf("\n");

        shivver_printl(obj);
        printf("\n");
}


