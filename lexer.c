#include "lexer.h"

// TODO: use strtok?
struct token_list* get_tok(char* buffer, size_t buffer_len) {
    // This is a special buffer. It should always end in ' ' so we don't
    // overshoot when parsing the last token
    assert(buffer[buffer_len] == ' ');
    struct token_list* tk_list = make_token_list();
    struct token *new_tok;
    char* token_begin = 0;
    for (size_t i = 0; i < buffer_len; i++) {
        // If we find whitespace, generate a new token
        if (buffer[i] == ' ') {
            // Token strings should end in \0
            buffer[i] = '\0';
            
            // TODO: test for strings!
            if (token_begin[0] == '#') { //if it's a comment, skip to the next line
                return tk_list;
            }
            // Generate the new token and append it to the token list
            new_tok = gen_tok(token_begin);
            if (new_tok != NULL)
                append_token_list(tk_list, new_tok);
            // the next token will begin after this
            token_begin = buffer + i + 1;
        }
    }
    return tk_list;
}

struct token* gen_tok(char* new_token) {
    // If the new token string is empty, don't generate a token.
    if (new_token[0] == '\0') {
        return NULL;
    }
    struct token *tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    if (is_num(new_token)) {
        tk->type = tok_number;
        tk->value.dbl = strtod(new_token, NULL);
        return tk;
    } else if (is_string(new_token)) { // TODO: This was a terrible idea
        tk->type = tok_string;
        tk->value.string = malloc(strlen(new_token));
        strcpy(tk->value.string, new_token);
        return tk;
    } else if (is_func(new_token)) {
        tk->type = tok_extern; 
    } else if (is_extern(new_token)) {
        tk->type = tok_identifier;
    } else if (is_identifier(new_token)) {
        tk->type = tok_identifier;
        tk->value.string = malloc(strlen(new_token));
        strcpy(tk->value.string, new_token);
    } else if (new_token[1] == '\0') { // it must be punctuation
        tk->type = tok_identifier;
        tk->value.string = malloc(2);
        tk->value.string[0] = new_token[0];
        tk->value.string[1] = '\0';
    } else {
        printf("Invalid token %s!\n", new_token);
        exit(-1);
    }
    return tk;
}

// TODO: handle _,-?
bool is_identifier(char *test_str) {
    for (size_t i = 0; test_str[i] != '\0'; i++) {
        if (!(isalnum(test_str[i]))) {
            return false;
        }
    }
    return true;
}

bool is_extern(char *test_str) {
    if (strcmp(test_str, "extern") != 0) {
        return false;
    }
    return true;
}

bool is_func(char *test_str) {
    if (strcmp(test_str, "func") != 0) {
        return false;
    }
    return true;
}

// TODO: handle string escaping
bool is_string(char *test_str) {
    if (test_str[0] != '"') return false;
    for (size_t i = 0; test_str[i] != '\0'; i++) {
        if (test_str[i] == '"' && test_str[i+1] != '\0') {
            return false;
        }
    }
    return true;
}

// TODO: 1.1.1 is valid. it shouldn't be.
bool is_num(char *test_str) {
    for (size_t i = 0; test_str[i] != '\0'; i++) {
        if (!(isdigit(test_str[i]) || test_str[i] == '.')) {
            return false;
        }
    }
    return true;
}

struct token_list* make_token_list() {
    struct token_list *new_list = malloc(sizeof(struct token));
    new_list->head = NULL;
    new_list->tail = NULL;
    return new_list;
}

void append_token_list(struct token_list* tk_list, struct token* new_token) {
    if (tk_list->head == NULL) {
        tk_list->head = new_token;
    }
    if (tk_list->tail != NULL) {
        tk_list->tail->next_token = new_token;
    }
    tk_list->tail = new_token;
    new_token->next_token = NULL;
}

void destroy_token_list(struct token_list *tk_list) {
    struct token *prev_tok = NULL;
    struct token *cur_tok = tk_list->head;
    while (cur_tok != NULL) {
        prev_tok = cur_tok;
        cur_tok = cur_tok->next_token;
        free(prev_tok);
    }
}
