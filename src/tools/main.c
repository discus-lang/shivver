
#include <stdlib.h>
#include <stdio.h>
#include "shivver/runtime.h"




int main(int argc, char** argv)
{
        shivver_heapInit(1024);
        obj_t* obj
         = aApvH( aAbsH ( 2
                        , (obj_t*[]){aSymT("x"), aSymT("y")}
                        , aMmmH (2, (obj_t*[]){aVarT("y", 0), aVarT("y", 0)}))
                , aMmmH (2, (obj_t*[]){aSymT("fresh"), aSymT("fish")}));


        shivver_printp(obj);
        printf("\n");

        shivver_printl(obj);
        printf("\n");

        obj_t* objE = shivver_eval(0, obj);
        shivver_printl(objE);
        printf("\n");

}


