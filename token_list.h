#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include <stdlib.h>
#include "token.h"

struct token_list {
    struct token *head;
    struct token *tail;
};

struct token_list* make_token_list();
struct token *pop_token_list(struct token_list* tkl);
struct token *peek_token_list(struct token_list* tkl);
void append_token_list(struct token_list* tk_list, struct token* new_token);
void destroy_token_list(struct token_list *tk_list);
#endif
