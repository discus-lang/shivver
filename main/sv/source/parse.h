
#pragma once
#include "sv/source/term.h"
#include "sv/token.h"

// Parser state.
typedef struct {
        // State of the token scanner.
        sv_token_state_t scanner;

        // Current token on input.
        sv_token_t here;

        // Next token for lookehead.
        sv_token_t next;
} sv_source_parse_t;


// from source/parse/base.c
sv_source_parse_t*
sv_source_parse_alloc(char* input);

void
sv_source_parse_fail(
        sv_source_parse_t* state,
        char* foramt,
        ...);

void
sv_source_parse_shift(
        sv_source_parse_t* state);

void
sv_source_parse_token(
        sv_source_parse_t* state,
        sv_token_tag_t tag);


// from source/parse/binders.c
sv_source_binders_t*
sv_source_parse_binders(
        sv_store_region_t* region,
        sv_source_parse_t* state);


// from source/parse/term.c
sv_source_term_t*
sv_source_parse_term(
        sv_store_region_t* region,
        sv_source_parse_t* state);

sv_source_term_t*
sv_source_parse_term0(
        sv_store_region_t* region,
        sv_source_parse_t* state);
