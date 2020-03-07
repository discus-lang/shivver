
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


//        char* str       = "(%fish)";
//        lexer_t state;
//        state.buf       = str;
//        state.len       = strlen(str);
//        state.pos       = 0;
//        state.prev      = 0;
//        obj_t* objP     = shivver_parse_term(&state);
//        shivver_printl(objP);
//        printf("\n");

}


void    main_tokens(char* str)
{
        // Get a pointer to the last char in the buffer.
        size_t strLen   = strlen(str);

        size_t tag      = TOKEN_NONE;
        size_t len      = 0;

        do {    shivver_lexer_scan(str, strLen, &tag, &str, &len);

                switch(tag)
                { case TOKEN_VAR:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_var(str, len, buf);
                        buf[len] = 0;
                        printf("(%%var \"%s\")\n", buf);
                        free(buf);
                        break;
                  }

                  case TOKEN_SYM:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_symprm(str, len, buf);
                        buf[len] = 0;
                        printf("(%%sym \"%s\")\n", buf);
                        free(buf);
                        break;
                  }

                  case TOKEN_PRM:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_symprm(str, len, buf);
                        buf[len] = 0;
                        printf("(%%prm \"%s\")\n", buf);
                        free(buf);
                        break;
                  }

                  default:
                        printf("%%%s\n", shivver_token_name(tag));
                }

                str += len;
                strLen -= len;

        } while (  tag != TOKEN_NONE
                && tag != TOKEN_END);
}


