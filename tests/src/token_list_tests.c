#include "../../src/token.h"
#include "../../src/token_list.h"
#include "tests.h"

void test_make_token_list();
void test_destroy_token_list();
void test_append_token_list();

int main() {
    test_make_token_list();
    test_append_token_list();
    test_destroy_token_list();
    return RETURN_VALUE;
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
    free(new_token0);
    free(new_token1);
    free(tkl);
}

void test_destroy_token_list() {
    struct token* new_token0 = malloc(sizeof(struct token));
    struct token* new_token1 = malloc(sizeof(struct token));
    new_token0->type = tok_number;
    new_token1->type = tok_number;
    struct token_list *tkl = make_token_list();
    append_token_list(tkl, new_token0);
    append_token_list(tkl, new_token1);
    destroy_token_list(tkl);
}
