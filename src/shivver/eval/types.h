#pragma once
#include <setjmp.h>
#include "shivver/types.h"


// Linked list of top-level macro declarations.
typedef struct eval_decl_tt {
        // Pointer to the name object in the heap.
        obj_t*  name;

        // Pointer to the body expression in the heap;
        obj_t*  body;

        // Pointer to the next link in the chain,
        // or NULL if this is the final one.
        struct eval_decl_tt* next;
} eval_decl_t;


// Evaluator state.
typedef struct {

        // Where to jump to on evaluation error.
        jmp_buf jmp_err;

        // Error message string.
        //  If there has not been an error then this is 0.
        //  If there was an error during evaluation then this is set to
        //  the error message, which will be freed by shivver_eval_free.
        char* error_str;

        // Linked list of top-level declarations,
        // or NULL if there aren't any.
        eval_decl_t* decls;

} eval_t;


