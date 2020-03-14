
#include <stdarg.h>
#include <setjmp.h>
#include "shivver/eval.h"


// Allocate a new evaluation state.
//
//   The error handler in jmp_error is initialized to a default handler
//   that will just abort() if called.
eval_t*
shivver_eval_alloc
        ()
{
        eval_t* state = (eval_t*)malloc(sizeof(eval_t));
        state->error_str = 0;

        // Initialize the jump buffer to a default target that just aborts.
        // The caller of shivver_eval_alloc should overwrite jmp_error with
        // its own handler.
        int res = setjmp (state->jmp_err);
        if (res == 0)
        {       return state;
        }
        abort();
}


// Free an evaluation state.
void
shivver_eval_free
        (eval_t*        state)
{
        free(state);
}


// Throw an evaluation error if the given property is false.
void
shivver_eval_require
        ( eval_t*       state
        , bool          prop
        , char*         format
        , ...)
{
        if (!prop) {
                va_list args;
                va_start (args, format);
                shivver_eval_error (state, format, args);
                va_end (args);
        }
}


// Throw an evaluation error.
//
//  The string is printed into a new malloced buffer which will
//  be freed by the original caller of the evaluator.
void
shivver_eval_error
        ( eval_t*       state
        , char*         format
        , ...)
{
        // Print the message into a new buffer.
        char* str       = malloc(1024);

        va_list args;
        va_start (args, format);
        vsnprintf (str, 1024, format, args);
        va_end (args);

        // Set the error string, which will be freed by the
        // top-level caller to this evaluator.
        state->error_str = str;

        // Return to the caller of the top-level evaluator.
        longjmp (state->jmp_err, 1);
}