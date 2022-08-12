
#include <stdio.h>
#include "sv/source.h"


void term(char* string)
{
        size_t block_size = 4096;

        sv_store_region_t* region
         = sv_store_region_create(block_size);

        sv_source_parse_t* state
         = sv_source_parse_alloc(string);

        sv_source_term_t* term
         = sv_source_parse_term(region, state);

        sv_store_rope_t* rope
         = sv_source_pretty_term(region, term);

        char* result
         = sv_store_rope_toString(rope);

        printf("%s\n", result);
        free(result);
        sv_store_region_free(region);
}


int main(int argc, char* argv[])
{
        printf("-- names\n");
        term("variable");
        term("%symbol");
        term("#primitive");
        term("@macro");
        term("?nominal");
        printf("\n");

        printf("-- parens\n");
        term("(var)");
        term("(%sym)");
        term("(@mac)");
        term("((@mac))");
        printf("\n");

        printf("-- abstractions\n");
        term("{} body");
        term("{a} body");
        term("{a b} body");
        term("{~a} body");
        term("{~a ~b} body");
        term("{~a~b} body");
        term(" {  ~a ~b  } body");
        term(" {  ~ one   two ~ three ~ four five  } body");
        term("{} {} body");
        term(" { one}   { ~ two three}  body  ");
        term(" { one} ({ ~ two three}  body)");
        printf("\n");

        printf("-- argument lists\n");
        term("[]");
        term("[one]");
        term("[one, two]");
        term("[[one], [two, three]]");
        term("[[one], ([two, three]), four]");
        term("{a b} [[one], ([two, three]), four]");
        term("[[one], ( [ two, three ]),  four, [five, six]]");
        printf("\n");
}
