
#pragma once
#include "sv/store.h"
#include "sv/source/term.h"

// Context in which a term is pretty printed,
// which is used to control parenthesis.
typedef enum {
        sv_source_pretty_context_top,
        sv_source_pretty_context_fun,
        sv_source_pretty_context_arg,
} sv_source_pretty_context_t;


// from source/pretty/binders.c
sv_store_rope_t*
sv_source_pretty_binders(
        sv_store_region_t* region,
        sv_source_binders_t* binders);


// from source/pretty/term.c
sv_store_rope_t*
sv_source_pretty_term(
        sv_store_region_t* region,
        sv_source_pretty_context_t context,
        sv_source_term_t* term);

