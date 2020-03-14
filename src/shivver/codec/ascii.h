#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include "shivver/heap.h"
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
#define TOKEN_MAC       33
#define TOKEN_KEY       34

#define TOKEN_KEY_TERM  40
#define TOKEN_KEY_LET   41
#define TOKEN_KEY_REC   42
#define TOKEN_KEY_IN    43


// ----------------------------------------------------------------------------
typedef struct {
        char*   str;            // string to parse.
        size_t  len;            // remaining chars in string to parse.

        // the current token buffer.
        size_t  curr_tok;
        char*   curr_str;
        size_t  curr_len;

        // the peeked token buffer.
        size_t  peek_tok;
        char*   peek_str;
        size_t  peek_len;

        // Where to jump to on parse error.
        jmp_buf jmp_err;

        // Error message string.
        //  If there has not been an error then this is 0.
        //  If there was an error during parsing then this is set to the
        //  error message, which will be freed by shivver_parse_free.
        char*   error_str;
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
shivver_lexer_scan_signame
        (char* str, size_t strLen);

void
shivver_lexer_load_signame
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
        ( parser_t* state
        , size_t tok);


// from ascii/parse/decl.c
obj_t*
shivver_parse_decl
        (parser_t* state);


// from ascii/parse/term.c
obj_t*
shivver_parse_term_zero
        (parser_t* state);

obj_t*
shivver_parse_term1
        (parser_t* state);

obj_t*
shivver_parse_term0
        (parser_t* state);

objlist_t*
shivver_parse_termCommaList
        (parser_t* state);

objlist_t*
shivver_parse_varSpaceList
        (parser_t* state);

bool
shivver_parse_isTermStart
        (size_t tok);


// from ascii/parse/prim.c
obj_t*
shivver_parse_nat_lit
        (char* str);

obj_t*
shivver_parse_nat_prim
        (char* str);
