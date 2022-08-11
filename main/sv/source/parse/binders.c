
#include <assert.h>
#include <string.h>
#include "sv/source.h"
#include "sv/store.h"

// Parse a possibly empty sequence of binders.
//  returning 0 if the sequence is empty.
//
// Binders ::= '~' Var Binders
//          |  Var Binders
//          |  e
//
sv_source_binders_t*
sv_source_parse_binders(
        sv_store_region_t* region,
        sv_source_parse_t* state)
{
        // unannoted demand is strong
        sv_source_demand_t demand
         = sv_source_demand_strong;

        // Binders ::= . '~' Var Binders
        //          |  . Var Binders
        //          |  . e
        switch(state->here.super.tag) {
        case sv_token_atom_tilde:
                demand = sv_source_demand_weak;
                sv_source_parse_shift(state);
                break;

        case sv_token_name_var:
                break;

        default:
                return 0;
        }

        // Binders ::= '~' . Var Binders
        //          |  . Var Binders
        if(state->here.super.tag == sv_token_name_var) {
                sv_source_binders_t* binders
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_binders_t) + state->here.name.count);

                binders->demand = demand;
                binders->length = state->here.name.count;
                memcpy(binders->name, state->here.name.first, state->here.name.count);

                binders->next = 0;
                binders->next = sv_source_parse_binders(region, state);
                return binders;
        }

        // Fail case can only be if we have accepted '~' but there
        // is no following var name after it.
        assert(demand == sv_source_demand_weak);
        assert(false);
}