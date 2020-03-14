
#include "shivver/heap.h"
#include "shivver/prim.h"
#include "shivver/codec/ascii.h"


// Parse a literal natural number, like "#nat'23"
obj_t*
shivver_parse_nat_lit
        (char* str)
{
        size_t x;
        if (sscanf(str, "nat'%zu", &x) == 1)
                return aNatA(x);

        return 0;
}


// Parse a nat primtitive, or return 0 if it's not recognised.
obj_t*
shivver_parse_nat_prim
        (char* str)
{
        if (strcmp(str, "nat'add") == 0)        return aPrzA(PRIM_NAT_ADD);
        if (strcmp(str, "nat'sub") == 0)        return aPrzA(PRIM_NAT_SUB);
        if (strcmp(str, "nat'mul") == 0)        return aPrzA(PRIM_NAT_MUL);
        if (strcmp(str, "nat'div") == 0)        return aPrzA(PRIM_NAT_DIV);

        if (strcmp(str, "nat'eq")  == 0)        return aPrzA(PRIM_NAT_EQ);
        if (strcmp(str, "nat'lt")  == 0)        return aPrzA(PRIM_NAT_LT);
        if (strcmp(str, "nat'le")  == 0)        return aPrzA(PRIM_NAT_LE);
        if (strcmp(str, "nat'gt")  == 0)        return aPrzA(PRIM_NAT_GT);
        if (strcmp(str, "nat'ge")  == 0)        return aPrzA(PRIM_NAT_GE);

        return 0;
}
