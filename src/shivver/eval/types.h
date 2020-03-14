#pragma once
#include <setjmp.h>
#include "shivver/types.h"

typedef struct {

        // Where to jump to on evaluation error.
        jmp_buf jmp_err;

        // Error message string.
        //  If there has not been an error then this is 0.
        //  If there was an error during evaluation then this is set to
        //  the error message, which will be freed by shivver_eval_free.
        char*   error_str;
} eval_t;


