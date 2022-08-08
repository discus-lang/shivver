
#include "shivver/util.h"

void    shivver_fail(char* reason)
{
        fprintf(stderr, "shivver failed: %s\n", reason);
        abort();
}