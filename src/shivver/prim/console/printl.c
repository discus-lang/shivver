
#include "shivver/prim.h"


// Print the logical structure of the graph.
void
shivver_prim_console_printl
        (obj_t* obj)
{
        switch (xObj_tag(obj))
        { // atomic ---------------------------------------
          case TAG_VARA:
          {     printf("%s",    xVarA_name(obj));
                return;
          }

          case TAG_SYMA:
          {     printf("%%%s",  xSymA_name(obj));
                return;
          }

          case TAG_PRMA:
          {     printf("#%s",   xPrmA_name(obj));
                return;
          }

          case TAG_NATA:
          {     printf("#nat'%llu", xNatA(obj));
                return;
          }


          // hot ------------------------------------------
          case TAG_MMMH:
          {     printf("[");
                size_t len = xMmmH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_prim_console_printl(xMmmH_arg(obj, i));
                        if (len - i > 1) printf(", ");
                }
                printf("]");
                return;
          }

          case TAG_ABSH:
          {     printf("({");
                size_t len = xAbsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_prim_console_printl(xAbsH_parm(obj, i));
                        if (len - i > 1) printf(" ");
                }
                printf("} ");
                shivver_prim_console_printl(xAbsH_body(obj));
                printf(")");
                return;
          }

          case TAG_APPH:
          {     printf("(");
                shivver_prim_console_printl(xAppH_fun(obj));
                printf(" ");
                shivver_prim_console_printl(xAppH_arg(obj));
                printf(")");
                return;
          }

          case TAG_APSH:
          {     printf("(");
                shivver_prim_console_printl(xApsH_fun(obj));

                printf(" [");
                size_t len = xApsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_prim_console_printl(xApsH_arg(obj, i));
                        if (len - i > 1) printf(", ");
                }
                printf("]");
                printf(")");
                return;
          }

          case TAG_CLOH:
          {     printf("&cloh...");
                return;
          }

          case TAG_ENVH:
          {     printf("&envh...");
                return;
          }

          // static ---------------------------------------
          case TAG_VART:
          {     printf("%s", xVarT_name(obj));
                return;
          }

          case TAG_SYMT:
          {     printf("%%%s", xSymT_name(obj));
                return;
          }

          default:
          {     printf("&invalid %0x", xObj_tag(obj));
          }
        }
}
