#include "parser.h"
#include "ast.h"
#include "tests.h"

void test_parse_number();

int main() {
    test_parse_number();
    return 0;
}

struct token *make_token(enum token_type tk_type, char* str, double dbl,
        int number) {
    struct token *tk = malloc(sizeof(struct token));
    tk->type = tk_type;
    if (tk_type == tok_dbl) {
        tk->value.dbl = dbl;
    } else if (tk_type == tok_number) {
        tk->value.number = number;
    } else {
        tk->value.string = string;
    }
    return tk;
}

void test_parse_number() {
    struct token_list *tkl = make_token_list();
    tkl.append(make_token(tok_number, NULL, 0.0, 42);
    struct ast_node *result = parse_number(tkl);
    EXPECT_EQ(result->val, tkl->head);
    EXPECT_EQ(result->num_children, 0);
    destroy_token_list(tkl);
}