
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
        { case TAG_MMM:
          {     printf("[");
                size_t len = xMmm_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_printl(xMmm_arg(obj, i));
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

          case TAG_ABSM:
          {     printf("({");
                size_t len = xAbsM_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_printl_sym(xAbsM_parm(obj, i));
                        if (len - i > 1) printf(" ");
                }
                printf("} ");
                shivver_printl(xAbsM_body(obj));
                printf(")");
                break;
          }

          case TAG_APPS:
          {     printf("(");
                shivver_printl(xAppS_fun(obj));

                printf(" [");
                size_t len = xAppS_len(obj);
                for(size_t i = 0; i < len; i++)
                {       shivver_printl(xAppS_arg(obj, i));
                        if (len - i > 1) printf(", ");
                }
                printf("]");
                printf(")");
                break;
          }

          case TAG_APPV:
          {     printf("(");
                shivver_printl(xAppV_fun(obj));
                printf(" ");
                shivver_printl(xAppV_arg(obj));
                printf(")");
                break;
          }

          default:
                printf("&invalid");
        }
}
