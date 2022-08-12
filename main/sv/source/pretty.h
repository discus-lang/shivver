
#pragma once
#include "sv/store.h"
#include "sv/source/term.h"


// from source/pretty/binders.c
sv_store_rope_t*
sv_source_pretty_binders(
        sv_store_region_t* region,
        sv_source_binders_t* binders);


// from source/pretty/term.c
sv_store_rope_t*
sv_source_pretty_term(
        sv_store_region_t* region,
        sv_source_term_t* term);

