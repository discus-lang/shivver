#pragma once
#include "sv/token.h"

// ------------------------------------------------------------------------------------------------
union sv_source_exp_t_;

typedef enum {
        sv_source_name_var,
        sv_source_name_sym,
        sv_source_name_prm,
        sv_source_name_mac,
        sv_source_name_nom,

        sv_source_lit_nat
} sv_source_exp_tag_t;


typedef struct {
        sv_token_range_t range;
        size_t  length;
} sv_source_exp_super_t;


typedef struct {
        sv_token_range_t range;
        size_t  length;
        char    name[];
} sv_source_exp_name_t;


typedef struct {
        sv_token_range_t range;
        union sv_source_exp_t_* fun;
        union sv_source_exp_t_* arg;
} sv_source_exp_app_t;


typedef struct {
        sv_token_range_t range;
        size_t count;
        union sv_source_exp_t_* arg;
} sv_source_exp_mmm_t;


// ------------------------------------------------------------------------------------------------
typedef struct {
        // State of the token scanner.
        sv_token_state_t scanner;

        // Current token on input.
        sv_token_t here;

        // Next token for lookehead.
        sv_token_t next;
} sv_source_parser_t;


// from source/base.c
sv_source_parser_t*
sv_source_parser_alloc(char* input);

void
sv_source_parser_fail(
        sv_source_parser_t* state,
        char* foramt,
        ...);

void
sv_source_parser_shift(
        sv_source_parser_t* state);


