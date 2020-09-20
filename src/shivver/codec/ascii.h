#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <alloca.h>
#include "shivver/heap.h"
#include "shivver/util.h"

// ----------------------------------------------------------------------------
// Meta token tags.
#define TOKEN_NONE              0
#define TOKEN_END               1

// Concrete token tags.
#define TOKEN_RBRA              10
#define TOKEN_RKET              11
#define TOKEN_CBRA              12
#define TOKEN_CKET              13
#define TOKEN_SBRA              14
#define TOKEN_SKET              15

#define TOKEN_COMMA             20
#define TOKEN_SEMI              21
#define TOKEN_EQ                22

#define TOKEN_VAR               30
#define TOKEN_SYM               31
#define TOKEN_PRM               32
#define TOKEN_MAC               33
#define TOKEN_KEY               34
#define TOKEN_NAT               35

#define TOKEN_KEY_SHIVVER       40
#define TOKEN_KEY_DEF           41
#define TOKEN_KEY_LET           42
#define TOKEN_KEY_REC           43
#define TOKEN_KEY_IN            44


// ----------------------------------------------------------------------------
// Parser state.
typedef struct {
        // Pointer to next character to parse, or if there are no more
        // characters, to the next byte after the string.
        char*   str;

        // Number of remaining characters in the string to parse.
        size_t  len;

        // Current token buffer.
        //  Token that we have shifted into the state,
        //  or 0 0 0 if no token has been shifted yet.
        size_t  curr_tok;
        char*   curr_str;
        size_t  curr_len;

        // Peeked token buffer.
        //  Token that has been peeked from the string,
        //  or 0 0 0 if no token has been peeked yet.
        size_t  peek_tok;
        char*   peek_str;
        size_t  peek_len;

        // Where to jump to on parse error.
        jmp_buf jmp_err;

        // Error message string.
        //  If there has not been an error then 0.
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
shivver_lexer_scan_nat
        (char* str, size_t strLen);

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

void    shivver_parse_free
        (parser_t* state);

void
shivver_parse_fail
        ( parser_t*     state
        , char*         format
        , ...)
        __attribute__((noreturn));

void    shivver_parse_peek
        (parser_t* state);

void    shivver_parse_shift
        (parser_t* state);

void    shivver_parse_tok
        (parser_t* state, size_t tok);

obj_t*  shivver_parse_var
        (parser_t* state);

obj_t*  shivver_parse_mac
        (parser_t* state);


// from ascii/parse/module.c
obj_t*  shivver_parse_module_zero
        (parser_t* state);

obj_t*  shivver_parse_module
        (parser_t* state);


// from ascii/parse/decl.c
obj_t*  shivver_parse_decls
        (parser_t* state);

void    shivver_parse_decls_list
        (parser_t* state, objlist_t* list);

obj_t*  shivver_parse_decl
        (parser_t* state);


// from ascii/parse/term.c
obj_t*  shivver_parse_term_zero
        (parser_t* state);

obj_t*  shivver_parse_term1
        (parser_t* state);

obj_t*  shivver_parse_term0
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
obj_t*  shivver_parse_nat_lit   (char* str);
obj_t*  shivver_parse_nat_prim  (char* str);
