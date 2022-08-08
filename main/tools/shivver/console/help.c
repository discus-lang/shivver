
#include "tools/shivver/main.h"


void
shivver_console_cmd_help ()
{
        printf("Commands available from the prompt:\n");
        printf("\n");
        printf("  <term>          evaluate the given term.\n");
        printf("  :help, :?       show this help page.\n");
        printf("  :decls          list all top-level declarations.\n");
        printf("  :decl   NAME    print the declaration with the given name.\n");
        printf("  :printp TERM    print the physical format of the given term.\n");
        printf("\n");
}