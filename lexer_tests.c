#include "lexer.h"
#include "tests.h"


int exit_called_with = 0;
// Monkey patch exit()
void exit(int status) {
    exit_called_with = status;
}


void test_make_token_list();
void test_destroy_token_list();
void test_append_token_list();
void test_gen_tok();
void test_is_identifier();
void test_is_num();
void test_is_string();
void test_is_extern();
void test_is_func();


int main() {
    test_make_token_list();
    test_append_token_list();
    test_destroy_token_list();
    test_gen_tok();
    test_is_identifier();
    test_is_num();
    test_is_string();
    test_is_extern();
    test_is_func();
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

void test_gen_tok(char* new_token) {
    struct token *received;
    char *tok;

    tok = "";
    received = gen_tok(tok);
    EXPECT_EQ(received, NULL);

    tok = "1.23";
    received = gen_tok(tok);
    EXPECT_EQ(received->type, tok_number);
    EXPECT_EQ(received->value.dbl, 1.23);
    EXPECT_EQ(received->next_token, NULL);

    tok = "\"abc def\"";
    received = gen_tok(tok);
    EXPECT_EQ(received->type, tok_string);
    EXPECT_EQ(received->value.dbl, 1.23);
    EXPECT_EQ(received->next_token, NULL);

    tok = "func";
    received = gen_tok(tok);
    EXPECT_EQ(received->type, tok_def);
    EXPECT_EQ(received->value.dbl, 1.23);
    EXPECT_EQ(received->next_token, NULL);

    tok = "abc123";
    received = gen_tok(tok);
    EXPECT_EQ(received->type, tok_identifier);
    EXPECT_EQ(received->value.string, "abc123");
    EXPECT_EQ(received->next_token, NULL);

    tok = "+";
    received = gen_tok(tok);
    EXPECT_EQ(received->type, tok_punc);
    EXPECT_EQ(received->value.string, "+");
    EXPECT_EQ(received->next_token, NULL);

    tok = "..";
    received = gen_tok(tok);
    EXPECT_EQ(exit_called_with, -1);
}

void test_is_identifier() {
    char *tok;
    tok = "abc";
    EXPECT_EQ(is_identifier(tok), true);
    tok = "a_b-c";
    EXPECT_EQ(is_identifier(tok), true);
    tok = "abc123";
    EXPECT_EQ(is_identifier(tok), true);
    tok = "123abc";
    EXPECT_EQ(is_identifier(tok), true);
    tok = "abc+123";
    EXPECT_EQ(is_identifier(tok), false);
}

void test_is_extern() {
    char *tok;
    tok = "extern";
    EXPECT_EQ(is_extern(tok), true);
    tok = "abc";
    EXPECT_EQ(is_extern(tok), false);
}

void test_is_func(char *test_str) {
    char *tok;
    tok = "func";
    EXPECT_EQ(is_func(tok), true);
    tok = "abc";
    EXPECT_EQ(is_func(tok), false);
}

void test_is_num() {
    char *tok;
    tok = "1.2.3";
    EXPECT_EQ(is_num(tok), false);
    tok = "1.23";
    EXPECT_EQ(is_num(tok), true);
    tok = ".23";
    EXPECT_EQ(is_num(tok), true);
    tok = "123";
    EXPECT_EQ(is_num(tok), true);
    tok = "0x123";
    EXPECT_EQ(is_num(tok), true);
}

void test_is_string() {
    char *tok;
    tok = "\"";
    EXPECT_EQ(is_string(tok), false);
    tok = "\"\"";
    EXPECT_EQ(is_string(tok), true);
    tok = "\"\\\"\\\"";
    EXPECT_EQ(is_string(tok), false);
    tok = "\"abc 123\"";
    EXPECT_EQ(is_num(tok), true);
}
