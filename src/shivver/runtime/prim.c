
#include <stdlib.h>
#include "shivver/runtime/prim.h"


char*
shivver_runtime_prim_name
        (uint32_t tag)
{
        switch(tag)
        { case PRIM_NAT_ADD:    return "nat'add";
          case PRIM_NAT_SUB:    return "nat'sub";
          case PRIM_NAT_MUL:    return "nat'mul";
          case PRIM_NAT_DIV:    return "nat'div";
          case PRIM_NAT_EQ:     return "nat'eq";
          case PRIM_NAT_LT:     return "nat'lt";
          case PRIM_NAT_LE:     return "nat'le";
          case PRIM_NAT_GT:     return "nat'gt";
          case PRIM_NAT_GE:     return "nat'ge";
        }
        return 0;
}