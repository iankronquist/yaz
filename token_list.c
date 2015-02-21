#include "token_list.h"

struct token_list* make_token_list() {
    struct token_list *new_list = malloc(sizeof(struct token));
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
    struct token *cur = tk_list->head;
    if (cur != NULL) {
        return tk_list->head->next_token;
    } else {
        return NULL;
    }
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
