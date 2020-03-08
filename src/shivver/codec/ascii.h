#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "shivver/runtime.h"
#include "shivver/util.h"

// ----------------------------------------------------------------------------
#define TOKEN_NONE      0
#define TOKEN_END       1

#define TOKEN_RBRA      10
#define TOKEN_RKET      11
#define TOKEN_CBRA      12
#define TOKEN_CKET      13
#define TOKEN_SBRA      14
#define TOKEN_SKET      15

#define TOKEN_COMMA     20

#define TOKEN_VAR       30
#define TOKEN_SYM       31
#define TOKEN_PRM       32


// ----------------------------------------------------------------------------
typedef struct {
        char*   str;
        size_t  len;    // remaining chars in string to parse.

        size_t  curr_tok;
        char*   curr_str;
        size_t  curr_len;

        size_t  peek_tok;
        char*   peek_str;
        size_t  peek_len;

} parser_t;


// ----------------------------------------------------------------------------
const char*
shivver_token_name
        (size_t tag);


// ----------------------------------------------------------------------------
bool
shivver_lexer_scan
        ( char* str             // pointer to next character of input string.
        , size_t  strLen        // length of string prefix to consider.
        , size_t* outTag        // output for token tag.
        , char**  outStr        // output for start of symbol.
        , size_t* outLen);      // output for token length.

size_t
shivver_lexer_scan_var
        (char* str, size_t strLen);

void
shivver_lexer_load_var
        (char* str, size_t len, char* out);

size_t
shivver_lexer_scan_symprm
        (char* str, size_t strLen);

void
shivver_lexer_load_symprm
        (char* str, size_t len, char* out);


// ----------------------------------------------------------------------------
// from ascii/parse/base.c
parser_t*
shivver_parse_alloc
        (char* str);

void
shivver_parse_free
        (parser_t* state);

void
shivver_parse_peek
        (parser_t* state);

void
shivver_parse_shift
        (parser_t* state);

void
shivver_parse_tok
        (parser_t* state, size_t tok);


// from ascii/parse/term.c
bool
shivver_parse_isTermStart
        (size_t tok);

obj_t*
shivver_parse_term
        (parser_t* state);

objlist_t*
shivver_parse_varSpaceList
        (parser_t* state);

objlist_t*
shivver_parse_termCommaList
        (parser_t* state);


