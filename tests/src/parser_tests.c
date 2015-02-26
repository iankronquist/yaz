#include "../../src/parser.h"
#include "../../src/ast.h"
#include "tests.h"

void test_parse_number();
void test_parse_paren();
void test_parse_identifier();

int main() {
    test_parse_number();
    test_parse_paren();
    test_parse_identifier();
    return RETURN_VALUE;
}

struct token *make_token(enum token_type tk_type, char* str, double dbl,
        int number) {
    struct token *tk = malloc(sizeof(struct token));
    tk->type = tk_type;
    if (tk_type == tok_dbl) {
        tk->value.dbl = dbl;
    } else if (tk_type == tok_number) {
        tk->value.num = number;
    } else {
        tk->value.string = str;
    }
    return tk;
}

void test_parse_number() {
    struct token_list *tkl = make_token_list();
    struct token *tk = make_token(tok_number, NULL, 0.0, 42);
    append_token_list(tkl, tk);
    struct ast_node *result = parse_number(tkl);
    EXPECT_EQ(result->val, tk);
    EXPECT_EQ(result->num_children, 0);
    destroy_token_list(tkl);
    delete_node(result);
}

void test_parse_paren() {
    struct token_list *tkl = make_token_list();
    struct token *tk0 = make_token(tok_punc, "(", 0.0, 0);
    struct token *tk1 = make_token(tok_number, NULL, 0.0, 42);
    struct token *tk2 = make_token(tok_punc, "+", 0.0, 0);
    struct token *tk3 = make_token(tok_number, NULL, 0.0, 24);
    struct token *tk4 = make_token(tok_punc, ")", 0.0, 0);
    append_token_list(tkl, tk0);
    append_token_list(tkl, tk1);
    append_token_list(tkl, tk2);
    append_token_list(tkl, tk3);
    append_token_list(tkl, tk4);
    struct ast_node *result = parse_paren(tkl);
    EXPECT_EQ(result->val, tk2);
    EXPECT_EQ(result->num_children, 2);
    EXPECT_EQ(result->children[0]->val, tk1);
    EXPECT_EQ(result->children[1]->val, tk3);
    EXPECT_EQ(result->children[0]->num_children, 0);
    EXPECT_EQ(result->children[1]->num_children, 0);
    destroy_token_list(tkl);
    delete_node(result);
}

void test_parse_identifier() {
    struct token_list *tkl = make_token_list();
    struct token *tk = make_token(tok_identifier, "_identifier123", 0.0, 0);
    append_token_list(tkl, tk);
    struct ast_node *result = parse_identifier(tkl);
    EXPECT_EQ(result->val, tk);
    EXPECT_EQ(result->num_children, 0);
    destroy_token_list(tkl);
    delete_node(result);

    struct token *tk0, *tk1, *tk2, *tk3, *tk4, *tk5;
    tkl = make_token_list();
    tk0 = make_token(tok_identifier, "_function123", 0.0, 0);
    tk1 = make_token(tok_punc, "(", 0.0, 0);
    tk2 = make_token(tok_identifier, "a", 0.0, 0);
    tk3 = make_token(tok_punc, ",", 0.0, 0);
    tk4 = make_token(tok_identifier, "b", 0.0, 0);
    tk5 = make_token(tok_punc, ")", 0.0, 0);
    result = parse_identifier(tkl);
    EXPECT_EQ(result->val, tk0);
    EXPECT_EQ(result->num_children, 2);
    EXPECT_EQ(result->children[0]->val, tk2);
    EXPECT_EQ(result->children[1]->val, tk4);
    EXPECT_EQ(result->children[0]->num_children, 0);
    EXPECT_EQ(result->children[1]->num_children, 0);
    destroy_token_list(tkl);
    delete_node(result);
}
