#pragma once
#include "shivver/types.h"

void
shivver_fail
        (char* reason)
        __attribute__((noreturn));


static inline void
require ( bool  prop
        , char* message)
{
        if (!prop) shivver_fail(message);
}


