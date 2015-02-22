#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "panic.h"
#include "parser.h"

const char* precedences[] = {"<", ">", "+", "-", "*"};
const int num_operators = sizeof(precedences);

struct ast_node *parse_number(struct token_list* tkl) {
    return make_node(pop_token_list(tkl), 0);
}

struct ast_node *parse_paren(struct token_list *tkl) {
    struct token *cur = pop_token_list(tkl);
    assert(cur->value.string[0] == '('); // double checking
    struct ast_node *expr = parse_expr(tkl);
    cur = pop_token_list(tkl);
    if (cur != NULL && cur->value.string[0] != ')') {
        parse_error(")", cur->value.string, "");
    }
    return expr;
}

struct ast_node *parse_identifier(struct token_list *tkl) {
    struct token *cur = pop_token_list(tkl);
    struct token *identifier = cur; // the identifier
    cur = peek_token_list(tkl); // get the next token

    // If the next token is a ( this is a function call
    if (cur->type == tok_punc && cur->value.string[0] == '(') {
        //discard the paren
        pop_token_list(tkl);
        // allocate the function call
        struct ast_node *func_call = make_node(identifier, 0);
        // for each function in the list
        while (true) {
            // parse the expression
            struct ast_node *expr = parse_expr(tkl);
            if (expr == NULL) return NULL;
            cur = pop_token_list(tkl);
            // if the next token is a paren, stop
            if (cur->type == tok_punc && cur->value.string[0] == ')') {
                break;
            } else if (cur->type == tok_punc && cur->value.string[0] != ',') {
                // but if it's also not a comma, error
                parse_error(") or ,", cur->value.string, "");
            }
            // Append the expression to the function call's chilren
            append_child(func_call, expr);
        }
    } else { // else it is just a variable
        return parse_variable(identifier);
    }
}

struct ast_node *parse_variable(struct token_list *tkl) {
    return make_node(pop_token_list(tkl), 0);
}

struct ast_node *parse_primary(struct token_list *tkl) {
    struct token *cur = peek_token_list(tkl);
    switch (cur->type) {
        case tok_number:
            return parse_number(tkl);
        case tok_identifier:
            return parse_identifier(tkl);
        case tok_punc:
            if (cur->value.string[0] == ')') {
                return parse_paren(tkl);
            }
        default:
            // TODO: better error handling
            panic("Unexpected token!", "parse_primary");
    }
    // NOTREACHED
    return NULL;
}

int get_precedence(char* op) {
    int precedence;
    for (precedence = 0; precedence < num_operators; precedence++) {
        if (strncmp(op, precedences[precedence], 2) == 0) {
            return precedence;
        }
    }
    panic("Unexpected operator!", "get_precedence");
    return -1;
}

struct ast_node *parse_expr(struct token_list *tkl) {
    struct ast_node *left_side = parse_primary(tkl);
    if (left_side == NULL) return NULL;
    return parse_bin_op_right_side(tkl, 0, left_side);
}

struct ast_node *parse_bin_op_right_side(struct token_list *tkl, int expr_prec, struct ast_node *left_side) {
    while (true) {
        struct token *cur = peek_token_list(tkl);
        if (cur != NULL && cur->type == tok_punc) {
            int precedence = get_precedence(cur->value.string);
            struct token* bin_op = pop_token_list(tkl);
            struct ast_node* right_side = parse_primary(tkl);
            if (right_side == NULL) return NULL;
            struct token *next_token = peek_token_list(tkl);
            int next_precedence = get_precedence(next_token->value.string);
            if (precedence < next_precedence) {
                right_side = parse_bin_op_right_side(tkl, precedence + 1,
                                                     right_side);
                if (right_side == NULL) return NULL;

            } else {
                left_side = make_node(bin_op, 2, left_side, right_side);
            }
        } else {
            return left_side;
        }
    }
}
// TODO: can this be merged with the function parsing code?
struct ast_node *parse_prototype(struct token_list *tkl) {
    struct token *cur = pop_token_list(tkl);
    struct token *identifier = cur; // the identifier
    cur = peek_token_list(tkl); // get the next token

    // If the next token is a ( this is a function call
    if (cur->type == tok_punc && cur->value.string[0] == '(') {
        //discard the paren
        pop_token_list(tkl);
        // allocate the function call
        struct ast_node *func_call = make_node(identifier, 0);
        // for each function in the list
        while (true) {
            // parse the expression
            struct ast_node *expr = parse_expr(tkl);
            if (expr == NULL) return NULL;
            cur = pop_token_list(tkl);
            // if the next token is a paren, stop
            if (cur->type == tok_punc && cur->value.string[0] == ')') {
                break;
            } else if (cur->type == tok_punc && cur->value.string[0] != ',') {
                // but if it's also not a comma, error
                parse_error(") or ,", cur->value.string, "");
            }
            // Append the expression to the function call's chilren
            append_child(func_call, expr);
        }
    }
    return cur;
}
