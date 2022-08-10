#pragma once
#include "sv/token.h"

// ------------------------------------------------------------------------------------------------
union sv_source_term_t_;

typedef enum {
        sv_source_name_var,
        sv_source_name_sym,
        sv_source_name_prm,
        sv_source_name_mac,
        sv_source_name_nom,

        sv_source_lit_nat
} sv_source_term_tag_t;


typedef struct {
        sv_token_range_t range;
        size_t  length;
} sv_source_term_super_t;


typedef struct {
        sv_token_range_t range;
        size_t  length;
        char    name[];
} sv_source_term_name_t;


typedef struct {
        sv_token_range_t range;
        union sv_source_term_t_* fun;
        union sv_source_term_t_* arg;
} sv_source_term_app_t;


typedef struct {
        sv_token_range_t range;
        size_t count;
        union sv_source_exp_t_* arg;
} sv_source_term_mmm_t;


typedef union {
        sv_source_term_super_t  super;
        sv_source_term_name_t   name;
        sv_source_term_app_t    app;
        sv_source_term_mmm_t    mmm;
} sv_source_term_t;


// ------------------------------------------------------------------------------------------------
typedef struct {
        // State of the token scanner.
        sv_token_state_t scanner;

        // Current token on input.
        sv_token_t here;

        // Next token for lookehead.
        sv_token_t next;
} sv_source_parse_t;


// ------------------------------------------------------------------------------------------------
// from source/base.c
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


