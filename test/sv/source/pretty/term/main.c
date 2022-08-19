
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
         = sv_source_pretty_term(region, sv_source_pretty_context_top, term);

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

        printf("-- application\n");
        term("one two");
        term("one two three");
        term("one two three four");
        term("(one two) three");
        term("one (two three)");
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
        term("({one} two three) four");
        term("{one} two (three four)");
        term("{one} (two three four)");
        printf("\n");

        printf("-- argument lists\n");
        term("[]");
        term("[one]");
        term("[one, two]");
        term("[[one], [two, three]]");
        term("[[one], ([two, three]), four]");
        term("{a b} [[one], ([two, three]), four]");
        term("[[one], ( [ two, three ]),  four, [five, six]]");
        printf("-- sugar\n");
        term("[a, b] & c = [a, b, c]");
        term("f [x1, x2] & g [x3, x4] & h [x5]");
        term("f ([x1, x2] & g [x3, x4] & h [x5])");
        term("f [x1, x2] & g ([x3, x4] & h [x5])");
        printf("\n");

        printf("-- let bindings\n");
        term("!let {x} = one two !in three x");
        term("!let x = one two !in three x");
        term("!let {x y z} = one !in two [x, y, z]");
        term("!let x = !let y = one !in two !in y [one]");
        printf("-- sugar\n");
        term("!let x = one !in !let y = two !in [x, y]");
        printf("\n");

        printf("-- rec bindings\n");
        term("!rec f = f one !in f two");
        term("!rec f = f one !and g = f two !and h = f (g three) !in f two");
        printf("\n");


        printf("-- box\n");
        term("!box one");
        term("!box []");
        term("!box {one} two three");
        term("one !box two three");
        printf("\n");

        printf("-- run\n");
        term("!run one");
        term("!run []");
        term("!run {one} two three");
        term("one !run two three");
        printf("\n");
}
