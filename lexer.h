#ifndef LEXER_H
#define LEXER_H
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "token_list.h"

typedef enum {
    IDENT,
    INT,
    DBL,
    STR,
    UNKNOWN,
    SYMBOL
} lex_state;

void panic(const char* message, char* context);
struct token_list* get_tok(char* buffer, size_t buffer_len);
struct token* mint_str(char *token_begin, char *token_end);
struct token* mint_int(char *token_begin, char *token_end);
struct token* mint_dbl(char *token_begin, char *token_end);
struct token* mint_ident(char *token_begin, char *token_end);
struct token* mint_symbol(char *token_begin, char *token_end);
#endif
