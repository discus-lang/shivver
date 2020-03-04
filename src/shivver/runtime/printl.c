
#include "shivver/runtime.h"

// Print the logical structure of symbols only,
// without the preceeding '%' symbol.
void    shivver_printl_sym(obj_t* obj)
{
        uint8_t tag = (uint64_t) (obj->header & 0x0ff);
        switch (tag)
        { case TAG_SYMT:
          {     printf("%s", xSymT_name(obj));
                break;
          }

          default:
                printf("&invalid");
        }

}


// Print the logical structure of the graph.
void    shivver_printl(obj_t* obj)
{
        switch (xObj_tag(obj))
        { case TAG_MMMH:
          {     printf("[");
                size_t len = xMmmH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_printl(xMmmH_arg(obj, i));
                        if (len - i > 1) printf(", ");
                }
                printf("]");
                break;
          }

          case TAG_SYMT:
          {     printf("%%%s", xSymT_name(obj));
                break;
          }

          case TAG_VART:
          {     printf("%s", xVarT_name(obj));
                break;
          }

          case TAG_ABSH:
          {     printf("({");
                size_t len = xAbsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_printl_sym(xAbsH_parm(obj, i));
                        if (len - i > 1) printf(" ");
                }
                printf("} ");
                shivver_printl(xAbsH_body(obj));
                printf(")");
                break;
          }

          case TAG_APVH:
          {     printf("(");
                shivver_printl(xApvH_fun(obj));
                printf(" ");
                shivver_printl(xApvH_arg(obj));
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

          default:
                printf("&invalid");
        }
}
