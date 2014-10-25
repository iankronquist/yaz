#ifndef LEXER_H
#define LEXER_H
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

enum token_type {
    tok_eof,
    tok_def,
    tok_extern,
    tok_identifier,
    tok_number,
    tok_string,
    tok_punc,
};

struct token_list {
    struct token *head;
    struct token *tail;
};

union value_holder {
    char *string;
    double dbl;
    int none;
};

struct token {
    enum token_type type;
    union value_holder value;
    struct token *next_token;
};


struct token_list* get_tok(char* buffer, size_t buffer_len);
struct token* gen_tok(char* new_token);
bool is_identifier(char *test_str);
bool is_extern(char *test_str);
bool is_string(char *test_str);
bool is_func(char *test_str);
bool is_num(char *test_str);
struct token_list* make_token_list();
void append_token_list(struct token_list* tk_list, struct token* new_token);
void destroy_token_list(struct token_list *tk_list);
#endif
