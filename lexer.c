#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "token.h"
#include "token_list.h"
#include "panic.h"

void print_token(struct token *tk) {
    if (tk == NULL) {
        puts("NULL TOKEN!");
        return;
    }
    switch (tk->type) {
        case tok_dbl:
            puts("dbl");
            break;
        case tok_identifier:
            puts("ident");
            printf("|%s|\n", tk->value.string);
            break;
        case tok_number:
            puts("num");
            break;
        case tok_string:
            puts("str");
            break;
        case tok_punc:
            puts("punc");
            printf("%s", tk->value.string);
            break;
        case tok_def:
            puts("def");
            break;
        case tok_extern:
            puts("extern");
            break;
        default:
            puts("Invalid token!");
            assert(0);
    }
}

struct token_list* get_tok(char* buffer, size_t buffer_len) {
    struct token_list* tk_list = make_token_list();
    char* token_begin = buffer;
    char* token_end = NULL;
    lex_state state = UNKNOWN;
    for (size_t index = 0; index < buffer_len; index++) {
        switch (state) {
            case UNKNOWN:
                token_begin = &buffer[index];
                if (isalpha(buffer[index]) || buffer[index] == '_') {
                    state = IDENT;
                } else if (isdigit(buffer[index])) {
                    state = INT;
                } else if (buffer[index] == '"') {
                    state = STR;
                } else if (buffer[index] != ' ') {
                    state = SYMBOL;
                }
                break;
            case IDENT:
                if (!isalnum(buffer[index]) && buffer[index] != '_') {
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_ident(token_begin, token_end));
                    state = UNKNOWN;
                }
                break;
            case INT:
                if (buffer[index] == '.') {
                    state = DBL;
                } else if (!isdigit(buffer[index])) {
                    state = UNKNOWN;
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_int(token_begin, token_end));
                }
                break;
            case DBL:
                if (!isdigit(buffer[index])) {
                    state = UNKNOWN;
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_dbl(token_begin, token_end));
                }
                break;
            case STR:
                if (&buffer[index] == buffer) {
                    panic("Lines can't start with a string", buffer);
                }
                if (buffer[index] == '"' && buffer[index-1] != '\\') {
                    state = UNKNOWN;
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_str(token_begin, token_end));
                }
                break;
            case SYMBOL:
                if (isalnum(buffer[index]) || buffer[index] == '_' ||
                        buffer[index] =='\n') {
                    state = UNKNOWN;
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_symbol(token_begin, token_end));
                }
                break;
        }
    }
    return tk_list;
}

struct token* mint_ident(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    struct token* tk = malloc(sizeof(struct token));
    if (strncmp(token_begin, "extern", 6) == 0) {
        tk->type = tok_extern;
        tk->value.string = NULL;
        tk->next_token = NULL;
    } else if (strncmp(token_begin, "def", 3) == 0) {
        tk->type = tok_def;
        tk->value.string = NULL;
        tk->next_token = NULL;
    } else {
        size_t len = token_end - token_begin;
        // + 1 for null byte
        char *receptacle = malloc(len + 1);
        tk->type = tok_identifier;
        tk->next_token = NULL;
        strncpy(receptacle, token_begin, len);
        tk->value.string = receptacle;
    }
    return tk;
}

struct token* mint_int(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    struct token* tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    tk->type = tok_number;
    tk->value.num = strtol(token_begin, NULL, 10);
    return tk;
}

struct token* mint_dbl(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    struct token* tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    tk->type = tok_dbl;
    tk->value.dbl = atof(token_begin);
    return tk;
}

struct token* mint_str(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    size_t len = token_end - token_begin;
    struct token* tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    // -2 to remove quotes, + 1 for null byte
    char *receptacle = malloc(len - 2 + 1);
    // constants to remove quotes
    strncpy(receptacle, token_begin + 1, len - 1);
    tk->type = tok_string;
    tk->value.string = receptacle;
    return tk;
}

struct token *mint_symbol(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    size_t len = token_end - token_begin;
    if (len > 2) {
        panic("Too many symbols!", token_begin);
    }
    struct token* tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    char *receptacle = malloc(2);
    strncpy(receptacle, token_begin, len);
    tk->type = tok_punc;
    tk->value.string = receptacle;
    return tk;
}
