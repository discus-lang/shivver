
#include "shivver/util.h"


// Strip the prefix from the line.
//  If the line has the prefix then return a pointer to
//  the first character after the prefix,
//  otherwise return null.
char*
shivver_string_stripPrefix
        (char* line, char* prefix)
{
        size_t i = 0;

  again:
        if (prefix[i] == 0)
                return line + i;

        if (line[i] == 0)
                return 0;

        if (line[i] != prefix[i])
                return 0;

        i++;
        goto again;

}

