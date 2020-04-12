
#include "shivver/prim.h"


// Print the logical structure of the graph.
void
shivver_prim_console_printl
        ( int           prec
        , obj_t*        obj)
{
        switch (xObj_tag(obj))
        { // atomic ---------------------------------------
          case TAG_VARA:
          {     printf("%s",    xVarA_name(obj));
                return;
          }

          case TAG_MACA:
          {     printf("@%s",   xMacA_name(obj));
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

          case TAG_PRZA:
          {     printf("#%s",   shivver_prim_nameOfTag(xPrzA_tag(obj)));
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
                {       shivver_prim_console_printl(0, xMmmH_arg(obj, i));
                        if (len - i > 1) printf(", ");
                }
                printf("]");
                return;
          }

          case TAG_ABSH:
          {     if(prec > 0) printf("(");
                printf("{");
                size_t len = xAbsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_prim_console_printl(0, xAbsH_parm(obj, i));
                        if (len - i > 1) printf(" ");
                }
                printf("} ");
                shivver_prim_console_printl(0, xAbsH_body(obj));
                if(prec > 0) printf(")");
                return;
          }

          case TAG_APPH:
          {     if(prec > 0) printf("(");
                shivver_prim_console_printl(1, xAppH_fun(obj));
                printf(" ");
                shivver_prim_console_printl(1, xAppH_arg(obj));
                if(prec > 0) printf(")");
                return;
          }

          case TAG_APSH:
          {     if (prec > 0) printf("(");
                shivver_prim_console_printl(1, xApsH_fun(obj));

                printf(" [");
                size_t len = xApsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_prim_console_printl(0, xApsH_arg(obj, i));
                        if (len - i > 1) printf(", ");
                }
                printf("]");
                if (prec > 0) printf(")");
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
