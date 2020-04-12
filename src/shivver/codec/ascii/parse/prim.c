
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
        uint32_t tag = shivver_prim_tagOfName(str);
        if (tag == 0)   return 0;
        else            return aPrzA(tag);
}
