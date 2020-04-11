
#include "tools/main.h"

// Command 'decls'.
//  Print out a list of all the top-level declarations.
void
shivver_console_cmd_decls
        ( eval_t*       state
        , char*         line)
{

        eval_decl_t* decl       = state->decls;
        while (decl != 0) {
                uint32_t len    = 0;
                char*    name   = 0;
                assert(dMac(decl->name, &len, &name));
                printf("%s\n", name);
                decl    = decl->next;
        }


}