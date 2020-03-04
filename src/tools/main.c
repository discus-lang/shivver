
#include <stdlib.h>
#include <stdio.h>
#include "shivver/runtime.h"
#include "shivver/codec/ascii.h"

void    main_tokens(char* str);

int main(int argc, char** argv)
{
        shivver_heapInit(1024);
        obj_t* obj
         = aApvH( aAbsH ( 2
                        , (obj_t*[]){aSymT("x"), aSymT("y")}
                        , aMmmH (2, (obj_t*[]){aVarT("y", 0), aVarT("x", 0)}))
                , aMmmH (2, (obj_t*[]){aSymT("fresh"), aSymT("fish")}));


        shivver_printp(obj);
        printf("\n");

        shivver_printl(obj);
        printf("\n");

        obj_t* objE = shivver_eval(0, obj);
        shivver_printl(objE);
        printf("\n");

        main_tokens("( ){ %fresh fish #derp }, ((");

}


void    main_tokens(char* str)
{
        lexer_t state;
        state.buf       = str;
        state.len       = strlen(str);
        state.pos       = 0;
        state.prev      = 0;

        size_t tag      = TOKEN_NONE;
        size_t len      = 0;

        do {    tag     = TOKEN_NONE;
                len     = 0;
                shivver_lexer_next(&state, &tag, &len);

                switch(tag)
                { case TOKEN_VAR:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_var(&state, buf);
                        printf("(%%var \"%s\")\n", buf);
                        free(buf);
                        break;
                  }

                  case TOKEN_SYM:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_symprm(&state, buf);
                        printf("(%%sym \"%s\")\n", buf);
                        free(buf);
                        break;
                  }

                  case TOKEN_PRM:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_symprm(&state, buf);
                        printf("(%%prm \"%s\")\n", buf);
                        free(buf);
                        break;
                  }

                  default:
                        printf("%%%s\n", shivver_token_name(tag));
                }

        } while (  tag != TOKEN_NONE
                && tag != TOKEN_END);
}


