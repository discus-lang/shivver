
#include "shivver/runtime.h"


// Print the logical structure of the graph.
void    shivver_printl(obj_t* obj)
{
        switch (xObj_tag(obj))
        { // hot ------------------------------------------
          case TAG_MMMH:
          {     printf("[");
                size_t len = xMmmH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_printl(xMmmH_arg(obj, i));
                        if (len - i > 1) printf(", ");
                }
                printf("]");
                break;
          }

          case TAG_VARH:
          {     printf("%s", xVarH_name(obj));
                break;
          }

          case TAG_SYMH:
          {     printf("%%%s", xSymH_name(obj));
                break;
          }

          case TAG_PRMH:
          {     printf("#%s", xSymH_name(obj));
                break;
          }

          case TAG_ABSH:
          {     printf("({");
                size_t len = xAbsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_printl(xAbsH_parm(obj, i));
                        if (len - i > 1) printf(" ");
                }
                printf("} ");
                shivver_printl(xAbsH_body(obj));
                printf(")");
                break;
          }

          case TAG_APPH:
          {     printf("(");
                shivver_printl(xAppH_fun(obj));
                printf(" ");
                shivver_printl(xAppH_arg(obj));
                printf(")");
                break;
          }

          case TAG_APSH:
          {     printf("(");
                shivver_printl(xApsH_fun(obj));

                printf(" [");
                size_t len = xApsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_printl(xApsH_arg(obj, i));
                        if (len - i > 1) printf(", ");
                }
                printf("]");
                printf(")");
                break;
          }

          case TAG_CLOH:
                printf("&cloh...");
                break;

          case TAG_ENVH:
                printf("&envh...");
                break;

          // static ---------------------------------------
          case TAG_VART:
          {     printf("%s", xVarT_name(obj));
                break;
          }

          case TAG_SYMT:
          {     printf("%%%s", xSymT_name(obj));
                break;
          }

          default:
                printf("&invalid %0x", xObj_tag(obj));
        }
}
