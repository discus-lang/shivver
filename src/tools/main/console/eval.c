
#include "tools/main.h"

// Command 'eval'
//  Parse the line as a term, evaluate it, and print the result.
void
shivver_console_cmd_eval
        ( eval_t*       state_eval      // evaluator state
        , char*         line)           // line to parse and evaluate.
{
        // Try to parse the term.
        parser_t* state_parse
         = shivver_parse_alloc(line);

        obj_t* obj_parse
         = shivver_parse_term_zero(state_parse);

        // If there was a parse error then obj_parse is set to 0,
        //  and the error message is set in the parse state.
        if (obj_parse == 0)
        {       printf("error: %s\n", state_parse->error_str);
                shivver_parse_free(state_parse);
                return;
        }
        shivver_parse_free(state_parse);

        obj_t* obj_eval
         = shivver_eval_term1_null (state_eval, 0, obj_parse);

        // If there was an eval error then obj_eval is set to 0,
        //  and the error message is set in the eval state.
        // The error message is built in malloced memory so we need to
        // free it after we've printed it.
        if (obj_eval == 0)
        {       assert(state_eval->error_str != 0);
                printf("error: %s\n", state_eval->error_str);
                free(state_eval->error_str);
                state_eval->error_str = 0;
                return;
        }

        // Evaluation completed successfully,
        //  so print the result.
        shivver_prim_console_printl (0, obj_eval);
        printf("\n");
}
