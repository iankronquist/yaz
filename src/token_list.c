#include "token_list.h"
#include <stdio.h>
#include <assert.h>

void print_token(struct token *tk) {
    if (tk == NULL) {
        printf("NULL TOKEN!\n");
        return;
    }
    switch (tk->type) {
        case tok_dbl:
            printf("dbl\n");
            break;
        case tok_identifier:
            printf("ident\n");
            printf("|%s|\n", tk->value.string);
            break;
        case tok_number:
            printf("num\n");
            break;
        case tok_string:
            printf("str\n");
            printf("|%s|\n", tk->value.string);
            break;
        case tok_punc:
            printf("punc\n");
            printf("%s", tk->value.string);
            break;
        case tok_def:
            printf("def\n");
            break;
        case tok_extern:
            printf("extern\n");
            break;
        default:
            printf("Invalid token!\n");
            assert(0);
    }
}


struct token_list* make_token_list() {
    struct token_list *new_list = malloc(sizeof(struct token_list));
    new_list->head = NULL;
    new_list->tail = NULL;
    return new_list;
}

struct token* pop_token_list(struct token_list* tk_list) {
    struct token *cur = tk_list->head;
    if (cur != NULL) {
        tk_list->head = tk_list->head->next_token;
    }
    return cur;
}

struct token* peek_token_list(struct token_list* tk_list) {
    return tk_list->head;
}

void append_token_list(struct token_list* tk_list, struct token* new_token) {
    if (tk_list->head == NULL) {
        tk_list->head = new_token;
    }
    if (tk_list->tail != NULL) {
        tk_list->tail->next_token = new_token;
    }
    tk_list->tail = new_token;
    new_token->next_token = NULL;
}

void destroy_token_list(struct token_list *tk_list) {
    struct token *prev_tok = NULL;
    struct token *cur_tok = tk_list->head;
    while (cur_tok != NULL) {
        prev_tok = cur_tok;
        cur_tok = cur_tok->next_token;
        if (prev_tok->type == tok_identifier ||
                prev_tok->type == tok_punc ||
                prev_tok->type == tok_string) {
            free(prev_tok->value.string);
        }
        free(prev_tok);
    }
    free(tk_list);
}
