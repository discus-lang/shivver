
#pragma once
#include "sv/token.h"
#include "sv/store.h"

/* --------------------------------------------------------------------------------------------- */
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
        // Link to the next binder, or 0 for end of list.
        struct sv_source_binders_t_* next;

        // Demand for the current binder.
        sv_source_demand_t demand;

        // Length of the current binder.
        size_t  length;

        // Characters in the current binder.
        char    name[];
} sv_source_binders_t;


/* --------------------------------------------------------------------------------------------- */
// Union of all possible term nodes.
union sv_source_term_t_;


// Binding list for recursive let-binding.
typedef struct sv_source_bindings_t_ {
        // Link fo the next binding, or 0 for end of list.
        struct sv_source_bindings_t_* next;

        // Body of the binding.
        union sv_source_term_t_* term;

        // Length of the binding name.
        size_t  length;

        // Characters in the binding name.
        char    name[];
} sv_source_bindings_t;


/* --------------------------------------------------------------------------------------------- */
// A list of terms.
typedef struct sv_source_term_list_t_ {
        // Pointer to the head term.
        union sv_source_term_t_* head;

        // Pointer to the next list cell, or 0 for end of list.
        struct sv_source_term_list_t_* tail;
} sv_source_term_list_t;


/* --------------------------------------------------------------------------------------------- */
// Tag to identify a term node.
typedef enum {
        sv_source_term_var,
        sv_source_term_sym,
        sv_source_term_prm,
        sv_source_term_mac,
        sv_source_term_nom,

        sv_source_term_lit_nat,

        sv_source_term_mmm,

        sv_source_term_app,
        sv_source_term_abs,

        sv_source_term_let,
        sv_source_term_rec,

        sv_source_term_box,
        sv_source_term_run
} sv_source_term_tag_t;


/* --------------------------------------------------------------------------------------------- */
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


// Term consisting of an argument list.
typedef struct {
        sv_token_range_t        range;
        sv_source_term_tag_t    tag;
        sv_source_term_list_t*  args;
} sv_source_term_mmm_t;


// Term consisting of an general application.
typedef struct {
        sv_token_range_t        range;
        sv_source_term_tag_t    tag;

        union sv_source_term_t_* fun;
        union sv_source_term_t_* arg;
} sv_source_term_app_t;


// Term consisting of an abstraction.
typedef struct {
        sv_token_range_t         range;
        sv_source_term_tag_t     tag;

        sv_source_binders_t*     binders;
        union sv_source_term_t_* body;;
} sv_source_term_abs_t;


// Term consisting of a high arity let binding.
typedef struct {
        sv_token_range_t         range;
        sv_source_term_tag_t     tag;

        sv_source_binders_t*     binders;
        union sv_source_term_t_* bound;
        union sv_source_term_t_* body;
} sv_source_term_let_t;


// Term consisting of recursive let bindings.
typedef struct {
        sv_token_range_t         range;
        sv_source_term_tag_t     tag;

        sv_source_bindings_t*    bindings;
        union sv_source_term_t_* body;
} sv_source_term_rec_t;


// Term consisting of a use of the '!box' keyword.
typedef struct {
        sv_token_range_t         range;
        sv_source_term_tag_t     tag;

        union sv_source_term_t_* body;
} sv_source_term_box_t;


// Term consisting of a use of the '!run' keyword.
typedef struct {
        sv_token_range_t         range;
        sv_source_term_tag_t     tag;

        union sv_source_term_t_* body;
} sv_source_term_run_t;


// Union of all possible term nodes.
typedef union sv_source_term_t_ {
        sv_source_term_super_t  super;
        sv_source_term_name_t   name;
        sv_source_term_app_t    app;
        sv_source_term_mmm_t    mmm;
        sv_source_term_abs_t    abs;
        sv_source_term_let_t    let;
        sv_source_term_rec_t    rec;
        sv_source_term_box_t    box;
        sv_source_term_run_t    run;
} sv_source_term_t;

