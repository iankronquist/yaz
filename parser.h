#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

// This could be a hash table, but those things are hefty
// Since this list is so short anyway the precedence is the index in the list
const char precedences[] = {'<', '>', '+', '-', '*'};
const int num_operators = sizeof(precedences);
int get_precedence(char op);
struct ast_node *parse_number(struct token_list* tkl);
struct ast_node *parse_paren(struct token_list *tkl);
struct ast_node *parse_identifier(struct token_list *tkl);
struct ast_node *parse_primary(struct token_list *tkl);
struct ast_node *parse_expr(struct token_list *tkl);
struct ast_node *parse_bin_op_right_side(struct token_list *tkl, int expr_prec, struct ast_node *left_side);
// TODO: can this be merged with the function parsing code?
struct ast_node *parse_prototype(struct token_list *tkl);

#endif
