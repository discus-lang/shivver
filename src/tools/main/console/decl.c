
#include "tools/main.h"
#include "shivver/eval.h"


// Command 'decls'.
//  Print out a list of all the top-level declarations.
void
shivver_console_cmd_decls
        ( eval_t*       state
        , char*         line)
{
        eval_decl_t* decl = state->decls;
        while (decl != 0) {
                uint32_t len    = 0;
                char*    name   = 0;
                assert(dMac(decl->name, &len, &name));
                printf("@%s\n", name);
                decl    = decl->next;
        }
}


// Command 'decl'.
//  Print out the a declaration.
void
shivver_console_cmd_decl
        ( eval_t*       state
        , char*         line)
{
        // skip over whitespace at the start of the line.
        while (*line != 0) {
                if (*line != ' ' && *line != '\t') break;
                line++;
        }

        // skip over the macro sigil.
        if (*line == '@') line++;

        obj_t* oDecl
         = shivver_eval_resolve_mac
                (state->decls, line);

        if (oDecl == 0) {
                printf("unknown\n");
                return;
        }

        shivver_prim_console_printl(0, oDecl);
        printf ("\n");
}

