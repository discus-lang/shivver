
#include <stdarg.h>
#include <setjmp.h>
#include <assert.h>
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

        // Initialize the jump buffer to a default target that just aborts.
        // The caller of shivver_eval_alloc should overwrite jmp_error with
        // its own handler.
        int res = setjmp (state->jmp_err);
        if (res == 0)
        {       return state;
        }
        shivver_fail("shivver_eval_alloc: eval error handler not set.");

        state->error_str = 0;
        state->decls     = 0;

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
        , va_list       args)
{
        // Print the message into a new buffer.
        char* str       = malloc(1024);
        vsnprintf (str, 1024, format, args);

        // Set the error string, which will be freed by the
        // top-level caller to this evaluator.
        state->error_str = str;

        // Return to the caller of the top-level evaluator.
        longjmp (state->jmp_err, 1);
}


// Ingest a module declaration into the evaluator state,
// so that we can refer to the defined macros during evaluation.
// This prepends the macros in the module decalration to the 'decls'
// part of the evaluator state.
void
shivver_eval_ingest
        ( eval_t*       state
        , obj_t*        oModule)
{
        obj_t*  osModule[2];
        assert(dSymAps(oModule, "svr'module", 2, osModule));

}