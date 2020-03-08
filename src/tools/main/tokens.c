#include "tools/main.h"


// Lex the given string as a term and print its tokens to console.
void    shivver_main_tokens(char* str)
{
        // Get a pointer to the last char in the buffer.
        size_t strLen   = strlen(str);

        size_t tag      = TOKEN_NONE;
        size_t len      = 0;

        while(true)
        {       shivver_lexer_scan(str, strLen, &tag, &str, &len);
                if ( tag == TOKEN_NONE
                 ||  tag == TOKEN_END)
                        return;

                switch(tag)
                { case TOKEN_VAR:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_var(str, len, buf);
                        buf[len] = 0;
                        printf("%%var \"%s\"\n", buf);
                        free(buf);
                        break;
                  }

                  case TOKEN_SYM:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_symprm(str, len, buf);
                        buf[len] = 0;
                        printf("%%sym \"%s\"\n", buf);
                        free(buf);
                        break;
                  }

                  case TOKEN_PRM:
                  {     char *buf = malloc(len + 1);
                        shivver_lexer_load_symprm(str, len, buf);
                        buf[len] = 0;
                        printf("%%prm \"%s\"\n", buf);
                        free(buf);
                        break;
                  }

                  default:
                        printf("%%%s\n", shivver_token_name(tag));
                }

                str += len;
                strLen -= len;
        }

}


