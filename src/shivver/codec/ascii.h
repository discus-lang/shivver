#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "shivver/runtime.h"

// ----------------------------------------------------------------------------
#define TOKEN_NONE      0
#define TOKEN_END       1

#define TOKEN_RBRA      10
#define TOKEN_RKET      11
#define TOKEN_CBRA      12
#define TOKEN_CKET      13

#define TOKEN_COMMA     20

#define TOKEN_VAR       30
#define TOKEN_SYM       31
#define TOKEN_PRM       32


// ----------------------------------------------------------------------------
const char*
shivver_token_name
        (size_t tag);


// ----------------------------------------------------------------------------
bool    shivver_lexer_scan
        ( char* str             // pointer to next character of input string.
        , size_t  strLen        // length of string prefix to consider.
        , size_t* outTag        // output for token tag.
        , char**  outStr        // output for start of symbol.
        , size_t* outLen);      // output for token length.

size_t  shivver_lexer_scan_var
        (char* str, size_t strLen);

void    shivver_lexer_load_var
        (char* str, size_t len, char* out);

size_t  shivver_lexer_scan_symprm
        (char* str, size_t strLen);

void    shivver_lexer_load_symprm
        (char* str, size_t len, char* out);


// ----------------------------------------------------------------------------
/*
obj_t*  shivver_parse_term
        (lexer_t* state);

obj_t*  shivver_parse_term1
        ( lexer_t* state
        , size_t tok, size_t len);

void    shivver_parse_tok
        (lexer_t* state, size_t tok);
*/