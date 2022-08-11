
#include <assert.h>
#include "sv/store/rope.h"
#include "sv/source.h"

sv_store_rope_t*
sv_source_pretty_term(
        sv_store_region_t* region,
        sv_source_term_t* term)
{
        switch(term->super.tag) {
        case sv_source_term_var:
                return sv_store_rope_fromString(region, term->name.name);

        case sv_source_term_sym:
                return sv_store_rope_printf(region, "%%%s", term->name.name);

        case sv_source_term_prm:
                return sv_store_rope_printf(region, "#%s", term->name.name);

        case sv_source_term_mac:
                return sv_store_rope_printf(region, "@%s", term->name.name);

        case sv_source_term_nom:
                return sv_store_rope_printf(region, "?%s", term->name.name);

        case sv_source_term_abs:
        {
                sv_store_rope_t* rBra
                 = sv_store_rope_fromString(region, "{");

                sv_store_rope_t* rBind
                 = sv_store_rope_join(region, rBra,
                        sv_source_pretty_binders(region,
                                term->abs.binders));

                sv_store_rope_t* rKey
                 = sv_store_rope_join(region, rBind,
                        sv_store_rope_fromString(region, "} "));

                sv_store_rope_t* rBody
                 = sv_store_rope_join(region, rKey,
                        sv_source_pretty_term(region, term->abs.body));

                return rBody;
        }

        default:
                assert(false);
        }
}

