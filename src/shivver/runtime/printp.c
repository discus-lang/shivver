
#include "shivver/runtime.h"

// Print the physical structure of the graph.
void    shivver_printp(obj_t* obj)
{
        switch (xObj_tag(obj))
        {
          // hot ----------------------------------------------------
          case TAG_MMMH:
          {     uint32_t len = xMmmH_len(obj);
                printf("(&mmmh %u", len);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_printp(xMmmH_arg(obj, i));
                }
                printf(")");
                break;
          }

          case TAG_VARH:
          {     printf  ("(&varh %u \"%s\" %u)"
                        , xVarH_len(obj), xVarH_name(obj), xVarH_bump(obj));
                break;
          }

          case TAG_SYMH:
          {     printf  ("(&symh %u \"%s\")"
                        , xSymH_len(obj), xSymH_name(obj));
                break;
          }

          case TAG_PRMH:
          {     printf  ("(&prmh %u \"%s\")"
                        , xPrmH_len(obj), xPrmH_name(obj));
                break;
          }

          case TAG_ABSH:
          {     printf("(&absh %u", xAbsH_len(obj));
                size_t len = xAbsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_printp(xAbsH_parm(obj, i));
                }
                printf(" ");
                shivver_printp(xAbsH_body(obj));
                printf(")");
                break;
          }

          case TAG_APSH:
          {     printf("(&apsh %u ", xApsH_len(obj));
                shivver_printp(xApsH_fun(obj));
                size_t len = xApsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_printp(xApsH_arg(obj, i));
                }
                printf(")");
                break;
          }

          case TAG_APVH:
          {     printf("(&apvh ");
                shivver_printp(xApvH_fun(obj));
                printf(" ");
                shivver_printp(xApvH_arg(obj));
                printf(")");
                break;
          }

          // static -------------------------------------------------
          case TAG_SYMT:
          {     printf  ("(&symt %u \"%s\")"
                        , xSymT_len(obj)
                        , xSymT_name(obj));
                break;
          }

          case TAG_VART:
          {     printf  ("(&vart %u \"%s\" %u)"
                        , xVarT_len(obj), xVarT_name(obj)
                        , xVarT_bump(obj));
                break;
          }


          default:
          {     printf("&invalid");
                break;
          }
        }
}


