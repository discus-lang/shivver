
#include <assert.h>
#include <stdio.h>
#include "sv/source.h"

sv_source_term_t*
sv_source_parse_term(
        sv_source_parse_t* state)
{
        switch(state->here.super.tag) {
         case sv_token_name_var:
                printf("var");
                break;

         default:
                assert(false);
        }

        assert(false);
}