
#include <assert.h>
#include "sv/store/rope.h"
#include "sv/source.h"

sv_store_rope_t*
sv_source_pretty_term(
        sv_store_region_t* region,
        sv_source_pretty_context_t context,
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

        case sv_source_term_app:
        {
                sv_store_rope_t* rope =
                 sv_source_pretty_term(region,
                        sv_source_pretty_context_fun, term->app.fun);

                rope = sv_store_rope_join(region, rope,
                 sv_store_rope_fromString(region, " "));

                rope = sv_store_rope_join(region, rope,
                 sv_source_pretty_term(region,
                        sv_source_pretty_context_arg, term->app.arg));

                if(context == sv_source_pretty_context_arg) {
                        return sv_store_rope_string_parens(region, rope);
                }
                else    return rope;
        }

        case sv_source_term_mmm:
        {
                sv_store_rope_t* rope =
                 sv_store_rope_fromString(region, "[");

                for(sv_source_term_list_t* link = term->mmm.args;
                    link != 0; link = link->tail) {
                        rope = sv_store_rope_join(region, rope,
                         sv_source_pretty_term(region,
                                sv_source_pretty_context_top, link->head));

                        if (link->tail != 0) {
                                rope = sv_store_rope_join(region, rope,
                                        sv_store_rope_fromString(region, ", "));
                        }
                }

                rope = sv_store_rope_join(region, rope,
                        sv_store_rope_fromString(region, "]"));

                return rope;
        }

        case sv_source_term_abs:
        {
                sv_store_rope_t* rope =
                 sv_store_rope_fromString(region, "{");

                rope = sv_store_rope_join(region, rope,
                 sv_source_pretty_binders(region,
                        term->abs.binders));

                rope = sv_store_rope_join(region, rope,
                 sv_store_rope_fromString(region, "} "));

                rope = sv_store_rope_join(region, rope,
                 sv_source_pretty_term(region,
                        sv_source_pretty_context_top, term->abs.body));

                if(context != sv_source_pretty_context_top) {
                        return sv_store_rope_string_parens(region, rope);
                }
                else    return rope;
        }

        case sv_source_term_box:
        {
                sv_store_rope_t* rope =
                 sv_store_rope_fromString(region, "!box ");

                rope = sv_store_rope_join(region, rope,
                 sv_source_pretty_term(region,
                        sv_source_pretty_context_top, term->box.body));

                if(context != sv_source_pretty_context_top) {
                        return sv_store_rope_string_parens(region, rope);
                }
                else    return rope;
        }

        case sv_source_term_run:
        {
                sv_store_rope_t* rope =
                 sv_store_rope_fromString(region, "!run ");

                rope = sv_store_rope_join(region, rope,
                 sv_source_pretty_term(region,
                        sv_source_pretty_context_top, term->box.body));

                if(context != sv_source_pretty_context_top) {
                        return sv_store_rope_string_parens(region, rope);
                }
                else    return rope;
        }

        default:
                assert(false);
        }
}

