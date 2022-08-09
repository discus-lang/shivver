
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "sv/token.h"

char* scan(char* sToken)
{
        sv_token_state_t state;
        memset(&state, 0, sizeof(sv_token_state_t));
        state.input             = sToken;
        state.length            = strlen(sToken);
        state.pos.line          = 1;
        state.pos.column        = 1;
        state.tab_columns       = 8;

        sv_token_t token;
        memset(&token, 0, sizeof(sv_token_t));
        bool result = sv_token_scan(&state, &token);
        if (!result) {
                fprintf(stderr,"scan failed for token '%s'\n", sToken);
                exit(1);
        }

        char* sShow = sv_token_show(token);
        printf("%s\n", sShow);
        return sShow;
}

int main(int argc, char* argv[])
{
        // keywords
        scan("!def");
        scan("!let");
        scan("!rec");
        scan("!in");

        // punc
        scan("(");
        scan(")");
        scan("{");
        scan("}");
        scan("[");
        scan("]");
        scan(",");
        scan(";");
        scan("=");

        // names
        scan("variable");

        scan("%symbol");
        scan("%some'symbol");

        scan("@macro");
        scan("@some'macro");

        scan("*nominal");
        scan("*some'nominal");

        scan("#primitive");
        scan("#nat'add");
        scan("#nat'add'again");

}