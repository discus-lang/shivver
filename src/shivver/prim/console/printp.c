
#include "shivver/runtime.h"
#include "shivver/prim.h"


// Print the physical structure of the graph.
void
shivver_prim_console_printp
        (obj_t* obj)
{
        switch (xObj_tag(obj))
        {
          // atomic ---------------------------------------
          case TAG_VARA:
          {     printf  ("(&vara %u \"%s\" %u)"
                        , xVarA_len(obj)
                        , xVarA_name(obj), xVarA_bump(obj));
                break;
          }

          case TAG_SYMA:
          {     printf  ("(&syma %u \"%s\")"
                        , xSymA_len(obj), xSymA_name(obj));
                break;
          }

          case TAG_PRMA:
          {     printf  ("(&prma %u \"%s\")"
                        , xPrmA_len(obj), xPrmA_name(obj));
                break;
          }

          case TAG_PRZA:
          {     char* name = shivver_runtime_prim_name(xPrzA_tag(obj));
                if (name != 0)
                {       printf ("(&prza \"%s\")", name);
                        return;
                }
                printf("&invalid");
                break;
          }

          case TAG_NATA:
          {     printf  ("(&nata %lld)"
                        , xNatA(obj));
                break;
          }

          // static ---------------------------------------
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

          // hot ------------------------------------------
          case TAG_MMMH:
          {     uint32_t len = xMmmH_len(obj);
                printf("(&mmmh %u", len);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_prim_console_printp(xMmmH_arg(obj, i));
                }
                printf(")");
                break;
          }


          case TAG_ABSH:
          {     printf("(&absh %u", xAbsH_len(obj));
                size_t len = xAbsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_prim_console_printp(xAbsH_parm(obj, i));
                }
                printf(" ");
                shivver_prim_console_printp(xAbsH_body(obj));
                printf(")");
                break;
          }

          case TAG_APPH:
          {     printf("(&apph ");
                shivver_prim_console_printp(xAppH_fun(obj));
                printf(" ");
                shivver_prim_console_printp(xAppH_arg(obj));
                printf(")");
                break;
          }

          case TAG_APSH:
          {     printf("(&apsh %u ", xApsH_len(obj));
                shivver_prim_console_printp(xApsH_fun(obj));
                size_t len = xApsH_len(obj);
                for(size_t i = 0; i < len; i++)
                {       printf(" ");
                        shivver_prim_console_printp(xApsH_arg(obj, i));
                }
                printf(")");
                break;
          }

          case TAG_CLOH:
          {     printf("(&cloh %u ...", xCloH_len(obj));
                printf(")");
                break;
          }

          case TAG_ENVH:
          {     printf("(&envh %u ...", xEnvH_len(obj));
                printf(")");
                break;
          }

          default:
          {     printf("&invalid");
                break;
          }
        }
}


