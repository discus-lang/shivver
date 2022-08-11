
#include <assert.h>
#include "sv/store/rope.h"
#include "sv/source.h"

sv_store_rope_t*
sv_source_pretty_term(
        sv_store_region_t* region,
        sv_source_term_t* term)
{
        switch(term->super.tag) {
        case sv_source_name_var:
                return sv_store_rope_fromString(
                        region, term->name.name);

        case sv_source_name_sym:
                return sv_store_rope_printf(
                        region, "%%%s", term->name.name);

        case sv_source_name_prm:
                return sv_store_rope_printf(
                        region, "#%s", term->name.name);

        case sv_source_name_mac:
                return sv_store_rope_printf(
                        region, "@%s", term->name.name);

        case sv_source_name_nom:
                return sv_store_rope_printf(
                        region, "?%s", term->name.name);

        default:
                assert(false);
        }
}

