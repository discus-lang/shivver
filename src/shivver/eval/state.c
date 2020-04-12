
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
        state->error_str = 0;
        state->decls     = 0;

        // Initialize the jump buffer to a default target that just aborts.
        // The caller of shivver_eval_alloc should overwrite jmp_error with
        // its own handler.
        int res = setjmp (state->jmp_err);
        if (res == 0)
        {       return state;
        }
        shivver_fail("shivver_eval_alloc: eval error handler not set.");
}


// Free an evaluation state.
void
shivver_eval_free
        (eval_t*        state)
{
        if (state->error_str != 0) free(state->error_str);
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
//
// The module expression has format.
//   Module ::= %svr'module [Frag, %svr'decls [Decl...]]
//   Decl   ::= %svr'decl   [MAC, EXP]
//
void
shivver_eval_ingest
        ( eval_t*       state
        , obj_t*        oModule)
{
        obj_t*  osModule[2];
        assert(dSymApsN(oModule, "svr'module", 2, osModule));

        uint32_t nDecls;
        assert(dSymAps(osModule[1], "svr'decls", &nDecls));

        obj_t*  osDecls[nDecls];
        assert(dApsArgsN(osModule[1], nDecls, osDecls));

        // Pointer to where to write the pointer to the next link.
        eval_decl_t** eval_decl = &state->decls;

        // Build an eval_decl_t link for each of the decls.
        for (uint32_t i = 0; i < nDecls; i++) {
                obj_t* osDecl[2];
                assert(dSymApsN(osDecls[i], "svr'decl", 2, osDecl));

                // Allocate a new link to record this declaration.
                eval_decl_t* link = (eval_decl_t*)malloc(sizeof(eval_decl_t));
                link->name = osDecl[0];
                link->body = osDecl[1];
                link->next = 0;

                // Add the new link to the chain.
                *eval_decl = link;

                // If there are more links then add them to the tail of the current one.
                eval_decl = &link->next;
        }

}