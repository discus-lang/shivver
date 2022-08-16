
#include <assert.h>
#include <string.h>
#include "sv/source.h"
#include "sv/store.h"

// Parse a non-empty sequence of term bindings.
//
// Bindings ::= Var '=' Term ('!and' Bindings)?
//
sv_source_bindings_t*
sv_source_parse_bindings(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        assert(state->here.super.tag == sv_token_name_var);

        char* first  = state->here.name.first;
        size_t count = state->here.name.count;

        sv_source_bindings_t* bindings
         = sv_store_region_alloc(region,
                sizeof(sv_source_bindings_t) + count + 1);

        // copy variable name to the bindings node.
        bindings->length = count;
        memcpy(bindings->name, first, count);
        bindings->name[count] = 0;
        sv_source_parse_shift(state);

        sv_source_parse_token(state, sv_token_atom_eq);

        sv_source_term_t* body
         = sv_source_parse_term(region, state);
        bindings->term = body;

        if (state->here.super.tag == sv_token_atom_and) {
                sv_source_parse_shift(state);

                sv_source_bindings_t* rest
                 = sv_source_parse_bindings(region, state);

                bindings->next = rest;
        }
        else {
                bindings->next = 0;
        }

        return bindings;
}
