
#include "tools/main.h"

// Command 'printp'.
//  Parse the line as a term, and print its physical representation.
void
shivver_console_cmd_printp
        (char* line)
{
        parser_t* state
         = shivver_parse_alloc(line);

        obj_t* obj
         = shivver_parse_term_zero(state);

        // If there was a parse error then obj is set to 0,
        if (obj == 0)
        {       printf("%s\n", state->error_str);
                shivver_parse_free(state);
                return;
        }

        // Line was parsed successfully,
        //  so print out its physical description.
        shivver_parse_free(state);
        shivver_prim_console_printp(obj);
        printf("\n");
}