#pragma once
#include <stdlib.h>
#include <stdbool.h>

// Tag identifying the sort of token.
typedef enum {
        sv_token_meta_none,
        sv_token_meta_end,

        sv_token_atom_shivver,
        sv_token_atom_def,
        sv_token_atom_let,
        sv_token_atom_rec,
        sv_token_atom_in,

        sv_token_atom_rbra,  sv_token_atom_rket,
        sv_token_atom_cbra,  sv_token_atom_cket,
        sv_token_atom_sbra,  sv_token_atom_sket,
        sv_token_atom_comma,
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
        size_t  line;
        size_t  column;
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
        sv_token_tag_t   tag;
        sv_token_range_t range;
} sv_token_super_t;


// Meta token describing some aspect of the input.
typedef struct {
        sv_token_tag_t   tag;
        sv_token_range_t range;
} sv_token_meta_t;


// Atomic token represened only by its tag.
typedef struct {
        sv_token_tag_t   tag;
        sv_token_range_t range;
} sv_token_atom_t;


// Reference to a source name in the input text.
typedef struct {
        sv_token_tag_t   tag;
        sv_token_range_t range;

        // Pointer to the first byte in the input text that
        // defines the name token.
        char*           first;

        // Number of bytes in the name token.
        size_t          count;
} sv_token_name_t;


// Literal natural number.
typedef struct {
        sv_token_tag_t     tag;
        sv_token_range_t   range;

        uint64_t        value;
} sv_token_lit_nat_t;


// Union of all token type.
typedef union {
        sv_token_super_t   super;
        sv_token_meta_t    meta;
        sv_token_atom_t    atom;
        sv_token_name_t    name;
        sv_token_lit_nat_t lit_nat;
} sv_token_t;

