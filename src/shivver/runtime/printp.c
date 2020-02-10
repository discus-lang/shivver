
#include "shivver/runtime.h"

// Print the physical structure of the graph.
void    shivver_printp(obj_t* obj)
{
        switch (xObj_tag(obj))
        { case TAG_MMM:
          {     uint32_t len = xMmm_len(obj);
                printf("(&mmm %u", len);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_printp(xMmm_arg(obj, i));
                }
                printf(")");
                break;
          }

          case TAG_SYMT:
          {     printf("(&symt \"%s\")", xSymT_name(obj));
                break;
          }

          case TAG_VART:
          {     printf  ("(&vart %zu \"%s\" %d)"
                        , xVarT_size(obj), xVarT_name(obj)
                        , xVarT_bump(obj));
                break;
          }

          case TAG_ABSM:
          {     printf("(&absm %u", xAbsM_len(obj));
                size_t len = xAbsM_len(obj);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_printp(xAbsM_parm(obj, i));
                }
                printf(" ");
                shivver_printp(xAbsM_body(obj));
                printf(")");
                break;
          }

          case TAG_APPS:
          {     printf("(&apps %u ", xAppS_len(obj));
                shivver_printp(xAppS_fun(obj));
                size_t len = xAppS_len(obj);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_printp(xAppS_arg(obj, i));
                }
                printf(")");
                break;
          }

          case TAG_APPV:
          {     printf("(&appv ");
                shivver_printp(xAppV_fun(obj));
                printf(" ");
                shivver_printp(xAppV_arg(obj));
                printf(")");
                break;
          }

          default:
          {     printf("&invalid");
                break;
          }
        }
}


