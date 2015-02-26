#include "../../src/lexer.h"
#include "../../src/token.h"
#include "tests.h"

void test_get_tok();

int main() {
    test_get_tok();
    return RETURN_VALUE;
}

void test_get_tok(char* new_token) {
    struct token_list *received;
    char *tok;
    tok = "\n";
    received = get_tok(tok, strlen(tok));
    EXPECT_EQ(received->head, NULL);
    destroy_token_list(received);

    tok = "# asdf\n";
    received = get_tok(tok, strlen(tok));
    EXPECT_EQ(received->head, NULL);
    destroy_token_list(received);

    tok = "1.23   \n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_dbl);
    EXPECT_EQ(received->head->value.dbl, 1.23);
    EXPECT_EQ(received->head->next_token, NULL);
    destroy_token_list(received);

    tok = "\"abc def\"\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_string);
    EXPECT_EQ_STR(received->head->value.string, "abc def");
    EXPECT_EQ(received->head->next_token, NULL);
    destroy_token_list(received);

    tok = "def\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_def);
    EXPECT_EQ(received->head->value.string, NULL);
    EXPECT_EQ(received->head->next_token, NULL);
    destroy_token_list(received);

    tok = "abc123\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_identifier);
    EXPECT_EQ_STR(received->head->value.string, "abc123");
    EXPECT_EQ(received->head->next_token, NULL);
    destroy_token_list(received);

    tok = "+\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_punc);
    EXPECT_EQ_STR(received->head->value.string, "+");
    EXPECT_EQ(received->head->next_token, NULL);
    destroy_token_list(received);

    tok = "!=\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_punc);
    EXPECT_EQ_STR(received->head->value.string, "!=");
    EXPECT_EQ(received->head->next_token, NULL);
    destroy_token_list(received);
}
