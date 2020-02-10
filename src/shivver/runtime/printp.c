#include "shivver/runtime.h"

void
shivver_printp(obj_t* obj)
{
        uint8_t tag = (uint64_t) (obj->header & 0x0ff);

        switch (tag)
        { case TAG_MMM:
          {     printf("(&mmm %d", xMmm_size(obj));
                size_t len = xMmm_size(obj);
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
          {     printf("(&vart %zu \"%s\")", xVarT_size(obj), xVarT_name(obj));
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


