
#include "shivver/codec/ascii.h"
#include "shivver/runtime.h"


obj_t*
shivver_parse_nat_lit
        (char* str)
{
        size_t x;
        if (sscanf(str, "nat'%zu", &x) == 1)
                return aNatA(x);

        return 0;
}