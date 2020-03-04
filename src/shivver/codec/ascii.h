#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


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


typedef struct {
        char*   buf;
        size_t  len;
        size_t  pos;
        size_t  prev;   // position of last token start.
} lexer_t;


const char*
shivver_token_name
        (size_t tag);


void    shivver_lexer_next
        ( lexer_t* state
        , size_t* outToken
        , size_t* outLen);

size_t  shivver_lexer_scan_var
        (lexer_t* state);

void    shivver_lexer_load_var
        (lexer_t* state, char* buf);

size_t  shivver_lexer_scan_symprm
        (lexer_t* state);

void    shivver_lexer_load_symprm
        (lexer_t* state, char* buf);

