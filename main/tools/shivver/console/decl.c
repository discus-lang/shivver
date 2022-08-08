
#include "tools/shivver/main.h"
#include "shivver/eval.h"


// Command 'decls'.
//  Print out a list of all the top-level declarations.
void
shivver_console_cmd_decls
        ( eval_t*       state
        , char*         line)
{
        eval_decl_t* decl = state->decls;

        // If there are no decls then say this explicitly,
        // rather than not doing anything at all.
        if (decl == 0) {
                printf("(no decls)\n\n");
                return;
        }

        // Print out the names of all the decls in turn.
        while (decl != 0) {
                uint32_t len    = 0;
                char*    name   = 0;
                assert(dMac(decl->name, &len, &name));
                printf("%s\n", name);
                decl    = decl->next;
        }
        printf("\n");
}



// Command 'decl'.
//  Print out the a declaration.
void
shivver_console_cmd_decl
        ( eval_t*       state
        , char*         line)
{
        // Skip over whitespace at the start of the line.
        while (*line != 0) {
                if (*line != ' ' && *line != '\t') break;
                line++;
        }

        // Skip over the macro sigil.
        if (*line == '@') line++;

        // Lookup the body of the declaration.
        obj_t* oDecl
         = shivver_eval_resolve_mac
                (state->decls, line);

        // We couldn't find it.
        if (oDecl == 0) {
                printf("Unknown declaration '%s'\n\n", line);
                return;
        }

        shivver_prim_console_printl(0, oDecl);
        printf ("\n\n");
}

