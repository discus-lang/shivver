
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
        // unannoted binder has strong demand.
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
                char* first  = state->here.name.first;
                size_t count = state->here.name.count;

                sv_source_binders_t* binders
                 = sv_store_region_alloc(region,
                        sizeof(sv_source_binders_t) + count + 1);

                // copy variable name into the binders node.
                binders->demand = demand;
                binders->length = count;
                memcpy(binders->name, first, count);
                binders->name[count] = 0;
                sv_source_parse_shift(state);

                // parse the rest of the binders list.
                binders->next = 0;
                binders->next = sv_source_parse_binders(region, state);
                return binders;
        }

        // Fail case can only be if we have accepted '~' but there
        // is no following var name after it.
        assert(demand == sv_source_demand_weak);
        assert(false);
}