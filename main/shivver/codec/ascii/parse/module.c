
#include "shivver/codec/ascii.h"


// Parse a module, returning zero on error.
//  On success, the term is returned directly.
//  On failure, we return 0 and the error message is in state->error_str.
//   The message string in state->error_str needs to be freed by the caller.
obj_t*
shivver_parse_module_zero
        (parser_t* state)
{
        int ret = setjmp(state->jmp_err);
        if (ret == 0)
                return shivver_parse_module (state);
        else {
                // We got longjmped to, so there was a parse error.
                // An error message need to have been set in the state.
                assert(state->error_str != 0);

                // Signal to the caller that there was an error by
                // returning 0.
                return 0;
        }
}


// Parse a module, performing a longjmp on error.
obj_t*
shivver_parse_module
        (parser_t* state)
{
        shivver_parse_tok(state, TOKEN_KEY_SHIVVER);
        obj_t* oVar     = shivver_parse_var(state);
        obj_t* oDecls   = shivver_parse_decls(state);
        return aApsH (2, aSymT("svr'module"), (obj_t*[]){oVar, oDecls});
}
