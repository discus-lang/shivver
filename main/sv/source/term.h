
#pragma once
#include "sv/token.h"
#include "sv/store.h"


// Demand for a binder.
typedef enum {
        // stong demand reduces argument to a value before binding it.
        sv_source_demand_strong,

        // weak demand reduces argument to an answer before binding it.
        sv_source_demand_weak
} sv_source_demand_t;


// A sequence of binders,
// each with their own demand on the expression being bound.
typedef struct sv_source_binders_t_ {
        // Demand for the current binder.
        sv_source_demand_t demand;

        // Link to the next binder, or 0 for end of the list.
        struct sv_source_binders_t_* next;

        // Length of the current binder.
        size_t  length;

        // Characters in the current binder.
        char    name[];
} sv_source_binders_t;


// Tag to identify a term node.
typedef enum {
        sv_source_term_var,
        sv_source_term_sym,
        sv_source_term_prm,
        sv_source_term_mac,
        sv_source_term_nom,

        sv_source_term_lit_nat,

        sv_source_term_abs
} sv_source_term_tag_t;


union sv_source_term_t_;


// Supertype of all terms.
typedef struct {
        sv_token_range_t        range;
        sv_source_term_tag_t    tag;
} sv_source_term_super_t;


// Term consisting of a single name.
typedef struct {
        sv_token_range_t        range;
        sv_source_term_tag_t    tag;

        // Length of the name buffer, not including the null terminator.
        size_t  length;

        // Null terminated name string.
        char    name[];
} sv_source_term_name_t;


// Term consisting of an abstraction.
typedef struct {
        sv_token_range_t        range;
        sv_source_term_tag_t    tag;

        sv_source_binders_t*     binders;
        union sv_source_term_t_* body;;
} sv_source_term_abs_t;


// Term consisting of an general application.
typedef struct {
        sv_token_range_t        range;
        sv_source_term_tag_t    tag;

        union sv_source_term_t_* fun;
        union sv_source_term_t_* arg;
} sv_source_term_app_t;


// Term consisting of an argument list.
typedef struct {
        sv_token_range_t        range;
        sv_source_term_tag_t    tag;

        size_t                   count;
        union sv_source_term_t_* arg;
} sv_source_term_mmm_t;


// Union of all possible term nodes.
typedef union sv_source_term_t_ {
        sv_source_term_super_t  super;
        sv_source_term_name_t   name;
        sv_source_term_abs_t    abs;
        sv_source_term_app_t    app;
        sv_source_term_mmm_t    mmm;
} sv_source_term_t;