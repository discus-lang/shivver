#pragma once
#include <stdlib.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------------------------------- */
// Sort of token.
typedef enum {
        sv_token_sort_none,
        sv_token_sort_meta,
        sv_token_sort_atom,
        sv_token_sort_name,
        sv_token_sort_lit
} sv_token_sort_t;


// Tag identifying the token.
typedef enum {
        sv_token_meta_none,
        sv_token_meta_end,

        sv_token_atom_def,
        sv_token_atom_let,
        sv_token_atom_rec,
        sv_token_atom_and,
        sv_token_atom_in,
        sv_token_atom_box,
        sv_token_atom_run,

        sv_token_atom_rbra,  sv_token_atom_rket,
        sv_token_atom_cbra,  sv_token_atom_cket,
        sv_token_atom_sbra,  sv_token_atom_sket,
        sv_token_atom_comma,
        sv_token_atom_tilde,
        sv_token_atom_semi,
        sv_token_atom_eq,

        sv_token_name_var,
        sv_token_name_sym,
        sv_token_name_prm,
        sv_token_name_mac,
        sv_token_name_nom,

        sv_token_lit_nat,
} sv_token_tag_t;


// Position of the first or final character in a token.
typedef struct {
        uint32_t line;
        uint32_t column;
} sv_token_pos_t;


// Range of input characters that define the token.
typedef struct {
        // Position of first character.
        sv_token_pos_t first;

        // Position of final character.
        sv_token_pos_t final;
} sv_token_range_t;


// Supertype of the token representations.
typedef struct {
        sv_token_range_t range;
        sv_token_sort_t  sort;
        sv_token_tag_t   tag;
} sv_token_super_t;


// Meta token describing some aspect of the input.
typedef struct {
        sv_token_range_t range;
        sv_token_sort_t  sort;
        sv_token_tag_t   tag;
} sv_token_meta_t;


// Atomic token represened only by its tag.
typedef struct {
        sv_token_range_t range;
        sv_token_sort_t  sort;
        sv_token_tag_t   tag;
} sv_token_atom_t;


// Reference to a source name in the input text.
typedef struct {
        sv_token_range_t range;
        sv_token_sort_t  sort;
        sv_token_tag_t   tag;

        // Pointer to the first byte in the text that defines the name in the token.
        // For names prefixed by sigils we skip over the sigil.
        // This pointer points into the source text, rather than to a buffer including
        // just the name, so there is no null terminating character.
        char*   first;

        // Number of bytes in the name.
        size_t  count;
} sv_token_name_t;


// Literal natural number.
typedef struct {
        sv_token_range_t range;
        sv_token_sort_t  sort;
        sv_token_tag_t   tag;
        size_t           value;
} sv_token_lit_nat_t;


// Union of all token type.
typedef union {
        sv_token_super_t   super;
        sv_token_meta_t    meta;
        sv_token_atom_t    atom;
        sv_token_name_t    name;
        sv_token_lit_nat_t lit_nat;
} sv_token_t;


// State of the tokenizer.
typedef struct {
        // Input string.
        char*   input;

        // Max length of input to consider.
        size_t  length;

        // Index to next character to consider.
        size_t  next;

        // Source position of next token start.
        sv_token_pos_t pos;

        // How many columns to count for the tab character.
        size_t  tab_columns;
} sv_token_state_t;


/* ---------------------------------------------------------------------------------------------- */
// from token/scan.c
bool    sv_token_scan(
        sv_token_state_t* state,
        sv_token_t*       out_token);

size_t  sv_token_scan_sigil_name(
        char* str, size_t strLen);

size_t  sv_token_scan_var(
        char* str, size_t strLen);

size_t  sv_token_scan_lit_nat(
        char* str, size_t strLen,
        size_t* out_value);

bool    sv_token_matches_keyword(
        char* keyword, char* str, size_t strLen,
        size_t* out_keywordLength);


// from token/show.c
char*   sv_token_show_tag(
        sv_token_tag_t tag);

char*   sv_token_show_pos(
        sv_token_pos_t pos);

char*   sv_token_show_range(
        sv_token_range_t range);

char*   sv_token_show(
        sv_token_t token);
