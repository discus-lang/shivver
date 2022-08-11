
#include <assert.h>
#include "sv/store/rope.h"
#include "sv/source.h"

// Pretty print a sequence of binders.
sv_store_rope_t*
sv_source_pretty_binders(
        sv_store_region_t* region,
        sv_source_binders_t* binders)
{
        assert(region != 0);

        if(binders == 0) {
                return sv_store_rope_fromString(region, "");
        }

        assert(binders != 0);

        char* sDemand = 0;
        switch(binders->demand) {
         case sv_source_demand_strong: sDemand = "";  break;
         case sv_source_demand_weak:   sDemand = "~"; break;
         default: assert(false);
        }

        char* sSpace;
        if (binders->next == 0) {
                sSpace = "";
        }
        else    sSpace = " ";

        sv_store_rope_t* sHere
         = sv_store_rope_printf(region,
                "%s%s%s", sDemand, binders->name, sSpace);

        if(binders->next == 0) {
                return sHere;
        }

        sv_store_rope_t* sTail
         = sv_store_rope_join(region, sHere,
                sv_source_pretty_binders(region,
                        binders->next));

        return sTail;
}