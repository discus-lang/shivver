
#include <stdlib.h>
#include <stdio.h>
#include "shivver/prim.h"
#include "shivver/eval.h"
#include "shivver/codec/ascii.h"


void
shivver_console_start
        (obj_t* oModule);

void
shivver_console_line
        ( eval_t*       state_eval
        , char*         line);

void
shivver_main_tokens
        (char* str);


void
shivver_console_cmd_help
        ();

void
shivver_console_cmd_eval
        ( eval_t*       state_eval
        , char*         line);

void
shivver_console_cmd_printp
        (char*  line);

void
shivver_console_cmd_decls
        ( eval_t*       state_eval
        , char*         line);

void
shivver_console_cmd_decl
        ( eval_t*       state_eval
        , char*         line);
