#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "panic.h"
#include "parser.h"

/**
    An array of strings. Each element is an operator. The index of the operator
    is its precedence level.
*/
const char* precedences[] = {"<", ">", "+", "-", "*"};
/**
    The number of operators in the precedences list.
*/
const int num_operators = 5;

/**
    @brief Takes a number token from the token list and produces a new ast_node
    @param A non-null token list.
    @return Returns an ast_node produced from the head token in the list.
*/
struct ast_node *parse_number(struct token_list* tkl) {
    return make_node(pop_token_list(tkl), 0);
}

/**
    @brief Parse a parenthetical expression from the token list.
    @param A non-null token list which should have an open paren token,
    some more tokens, and a close paren token.
    Note that if there is no closing parenthesis the function will call
    `parse_error` and the program will exit.
    @return Returns the ast_node representing the expression within the
    parentheses.
*/
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


/**
    @brief Parse an identifier token.
    @param Takes a non-null token list.
    Parses an identifier, which may be a function.
    @return Returns an ast_node representing a variable or a function.
*/
struct ast_node *parse_identifier(struct token_list *tkl) {
    struct token *identifier = pop_token_list(tkl);
    struct token *cur = peek_token_list(tkl); // get the next token

    // If the next token is a ( this is a function call
    if (cur != NULL && cur->type == tok_punc && cur->value.string[0] == '(') {
        //discard the paren
        pop_token_list(tkl);
        // allocate the function call
        struct ast_node *func_call = make_node(identifier, 0);
        // for each function in the list
        while (true) {
            // parse the expression
            struct ast_node *expr = parse_expr(tkl);

            if (expr == NULL)
                parse_error("Not NULL", "NULL", "parsing function identifier");

            // Append the expression to the function call's children
            append_child(func_call, expr);

            cur = pop_token_list(tkl);

            // if the next token is a paren, stop
            if (cur->type == tok_punc && cur->value.string[0] == ')') {
                pop_token_list(tkl);
                break;
            } else if (cur->type == tok_punc && cur->value.string[0] != ',') {
                // but if it's also not a comma, error
                parse_error(") or ,", cur->value.string, "");
            }
        }
        return func_call;
    } else { // else it is just a variable
        return make_node(identifier, 0);
    }
    // NOTREACHED
    assert(0);
    return NULL;
}


/**
    @brief The default state of the parser.
    The Parser may next enter 
    Note that if the parser encounters an unexpected token, it will panic, run
    and hide, and exit.
    @return An ast_node which may be a variable, identifier, or parenthetical
    expression.
*/
struct ast_node *parse_primary(struct token_list *tkl) {
    struct token *cur = peek_token_list(tkl);
    if (cur == NULL)
        return NULL;
    switch (cur->type) {
        case tok_number:
            return parse_number(tkl);
        case tok_identifier:
            return parse_identifier(tkl);
        case tok_punc:
            if (cur->value.string[0] == '(') {
                return parse_paren(tkl);
            }
        default:
            // TODO: better error handling
            panic("Unexpected token!", "parse_primary");
    }
    // NOTREACHED
    return NULL;
}

/**
    @brief Determine the precedence of an operator.
    Note that a traditional implementation would use a hash table, but hash
    tables are unnecessarily complex for this project. One may note that the
    asymptotic complexity of a hash table is O(1) whereas the linear search of
    my implementation is O(n), but since the operator precedence list only has
    about 6 elements the additional overhead of calculating the hash function 
    and performing the lookup may even be slower.
    @param A string representing an operator. It should be no more than 2
    characters long.
    @return An integer representing the precedence level of the operator.
*/
int get_precedence(char* op) {
    int precedence;
    for (precedence = 0; precedence < num_operators; precedence++) {
        if (strncmp(op, precedences[precedence], 2) == 0) {
            return precedence;
        }
    }
    return -1;
}

/**
    @brief Parse an expression.
    @param A list of tokens which still shouldn't be null.
    @return Returns an ast_node which represents a binary expression.
*/
struct ast_node *parse_expr(struct token_list *tkl) {
    struct ast_node *left_side = parse_primary(tkl);
    if (left_side == NULL) return NULL;
    return parse_bin_op_right_side(tkl, 0, left_side);
}

/**
    @brief Parse the right hand side of a binary expression.
    @param Takes a non-null list of tokens, an integer representing the
    precedence of the operator, and an ast_node representing the left hand side
    of the expression.
    @return Returns an ast_node representing a binary expression.
*/
struct ast_node *parse_bin_op_right_side(struct token_list *tkl, int expr_prec, struct ast_node *left_side) {
    while (true) {
        struct token *cur = peek_token_list(tkl);
        if (cur != NULL && cur->type == tok_punc) {
            int precedence = get_precedence(cur->value.string);
            if (precedence < expr_prec)
                return left_side;
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

/**
    @brief Parse a function prototype.
    @todo There is a lot of code which duplicates function parsing.
    @param A non-null function list, if you didn't guess.
    @return returns and ast_node representing a functio prototype
*/
// TODO: can this be merged with the function parsing code?
struct ast_node *parse_prototype(struct token_list *tkl) {
    struct token *cur = pop_token_list(tkl);
    struct token *identifier = cur; // the identifier
    if (cur->type != tok_identifier) {
        parse_error("Type tok_identifier", "something else", "context");
    }
    cur = peek_token_list(tkl); // get the next token
    // If the next token is a ( this is a function call
    if (cur->type == tok_punc && cur->value.string[0] == '(') {
        parse_error("(", "something else", "context");
    }
    //discard the paren
    pop_token_list(tkl);
    // allocate the function call
    struct ast_node *func_call = make_node(identifier, 0);
    // for each function in the list
    while (cur->type != tok_punc && cur->value.string[0] != ')') {
        struct ast_node *expr = parse_expr(tkl);
        cur = pop_token_list(tkl);
        if (cur->type == tok_punc &&
                (cur->value.string[0] != ',' || cur->value.string[0] != ')')) {
            // but if it's also not a comma, error
            parse_error(") or ,", cur->value.string, "");
        }
        // Append the expression to the function call's chilren
        append_child(func_call, expr);
    }
    return func_call;
}
