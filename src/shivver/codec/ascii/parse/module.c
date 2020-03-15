
#include "shivver/codec/ascii.h"


// Parse a module.
obj_t*
shivver_parse_module
        (parser_t* state)
{
        shivver_parse_tok(state, TOKEN_KEY_SHIVVER);
        obj_t* oVar     = shivver_parse_var(state);
        obj_t* oDecls   = shivver_parse_decls(state);
        return aApsH (2, aSymT("svr'module"), (obj_t*[]){oVar, oDecls});
}
