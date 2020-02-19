
#include "shivver/runtime.h"

// Print the raw memory layout of an object.
void    shivver_printm(obj_t* obj)
{
        printf("obj %p\n", obj);

        switch(xObj_tag(obj))
        { case TAG_MMMH:
          {     printf(" .tag    = mmmh\n");
                printf(" .size   = %u\n",       xMmmH_len(obj));

                uint32_t n = xMmmH_len(obj);
                for(uint32_t i = 0; i < n; i++)
                        printf("  prm[%d] = %p\n", i, xMmmH_arg(obj, i));
                break;
          }

          case TAG_SYMT:
          {     printf(" .tag    = symt\n");
                printf(" .size   = %u\n",       xSymT_size(obj));
                printf(" .name   = \"%s\"\n",   xSymT_name(obj));
                break;
          }

          case TAG_VART:
          {     printf(" .tag    = vart\n");
                printf(" .size   = %zu\n",      xVarT_size(obj));
                printf(" .name   = \"%s\"\n",   xVarT_name(obj));
                printf(" .bump   = %u\n",       xVarT_bump(obj));
                break;
          }

          case TAG_ABSH:
          {     printf(" .tag    = absh\n");
                printf(" .count  = %u\n",       xAbsH_count(obj));
                printf(" .body   = %p\n",       xAbsH_body(obj));

                uint32_t n = xAbsH_count(obj);
                for(uint32_t i = 0; i < n; i++)
                        printf("  prm[%d] = %p\n", i, xAbsH_parm(obj, i));
                break;
          }

          case TAG_CLOH:
          {     printf(" .tag    = cloh\n");
                printf(" .count  = %u\n",       xCloH_len  (obj));
                printf(" .env    = %p\n",       xCloH_env  (obj));
                printf(" .parmp  = %p\n",       xCloH_parmp(obj));
                printf(" .body   = %p\n",       xCloH_body (obj));
                break;
          }

          case TAG_ENVH:
          {     printf(" .tag    = envh\n");
                printf(" .count  = %u\n",       xEnvH_count(obj));
                printf(" .varp   = %p\n",       xEnvH_varp(obj));
                printf(" .valp   = %p\n",       xEnvH_valp(obj));

                uint32_t n = xEnvH_count(obj);
                for(uint32_t i = 0; i < n; i++)
                {       printf("  var[%i] = %p\n", i, xEnvH_var(obj, i));
                        printf("  val[%i] = %p\n", i, xEnvH_val(obj, i));
                }
                break;
          }

          default:
                printf(" tag: INVALID\n");
        }
}

