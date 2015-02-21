#include "lexer.h"
#include "token.h"
#include "tests.h"


int exit_called_with = 0;
// Monkey patch exit()
void exit(int status) {
    exit_called_with = status;
}


void test_make_token_list();
void test_destroy_token_list();
void test_append_token_list();
void test_get_tok();


int main() {
    test_make_token_list();
    test_append_token_list();
    test_destroy_token_list();
    test_get_tok();
    return 0;
}

void test_make_token_list() {
    struct token_list *tkl = make_token_list();
    EXPECT_EQ(tkl->head, NULL);
    EXPECT_EQ(tkl->tail, NULL);
    free(tkl);
}

void test_append_token_list() {
    struct token* new_token0 = malloc(sizeof(struct token));
    struct token* new_token1 = malloc(sizeof(struct token));
    new_token0->next_token = (struct token*)1; // something which isn't NULL
    new_token1->next_token = (struct token*)1; // something which isn't NULL
    struct token_list *tkl = make_token_list();
    append_token_list(tkl, new_token0);
    EXPECT_EQ(tkl->head, new_token0);
    EXPECT_EQ(tkl->tail, new_token0);
    EXPECT_EQ(new_token0->next_token, NULL);
    append_token_list(tkl, new_token1);
    EXPECT_EQ(tkl->head, new_token0);
    EXPECT_EQ(tkl->tail, new_token1);
    EXPECT_EQ(new_token0->next_token, new_token1);
    EXPECT_EQ(new_token1->next_token, NULL);
}

void test_destroy_token_list() {
    struct token* new_token0 = malloc(sizeof(struct token));
    struct token* new_token1 = malloc(sizeof(struct token));
    struct token_list *tkl = make_token_list();
    append_token_list(tkl, new_token0);
    append_token_list(tkl, new_token1);
    destroy_token_list(tkl);
}

void test_get_tok(char* new_token) {
    struct token_list *received;
    char *tok;

    tok = "\n";
    received = get_tok(tok, strlen(tok));
    EXPECT_EQ(received->head, NULL);

    tok = "1.23   \n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_dbl);
    EXPECT_EQ(received->head->value.dbl, 1.23);
    EXPECT_EQ(received->head->next_token, NULL);

    tok = "\"abc def\"\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_string);
    EXPECT_EQ_STR(received->head->value.string, "abc def");
    EXPECT_EQ(received->head->next_token, NULL);

    tok = "def\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_def);
    EXPECT_EQ(received->head->value.string, NULL);
    EXPECT_EQ(received->head->next_token, NULL);

    tok = "abc123\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_identifier);
    EXPECT_EQ_STR(received->head->value.string, "abc123");
    EXPECT_EQ(received->head->next_token, NULL);

    tok = "+\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_punc);
    EXPECT_EQ_STR(received->head->value.string, "+");
    EXPECT_EQ(received->head->next_token, NULL);

    tok = "!=\n";
    received = get_tok(tok, strlen(tok));
    print_token(received->head);
    EXPECT_EQ(received->head->type, tok_punc);
    EXPECT_EQ_STR(received->head->value.string, "!=");
    EXPECT_EQ(received->head->next_token, NULL);
}
