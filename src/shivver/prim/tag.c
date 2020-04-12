
#include <stdlib.h>
#include <string.h>
#include "shivver/prim/tag.h"

char*
shivver_prim_nameOfTag
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
          case PRIM_IF:         return "if";
        }
        return 0;
}

uint32_t
shivver_prim_tagOfName
        (char* name)
{
        // nat
        if      (strcmp(name, "nat'add") == 0)  return PRIM_NAT_ADD;
        else if (strcmp(name, "nat'sub") == 0)  return PRIM_NAT_SUB;
        else if (strcmp(name, "nat'mul") == 0)  return PRIM_NAT_MUL;
        else if (strcmp(name, "nat'div") == 0)  return PRIM_NAT_DIV;
        else if (strcmp(name, "nat'eq")  == 0)  return PRIM_NAT_EQ;
        else if (strcmp(name, "nat'lt")  == 0)  return PRIM_NAT_LT;
        else if (strcmp(name, "nat'le")  == 0)  return PRIM_NAT_LE;
        else if (strcmp(name, "nat'gt")  == 0)  return PRIM_NAT_GT;
        else if (strcmp(name, "nat'ge")  == 0)  return PRIM_NAT_GE;

        // control
        else if (strcmp(name, "if") == 0)       return PRIM_IF;

        return PRIM_NONE;
}