#ifndef YAZ_TOKEN_H
#define YAZ_TOKEN_H

enum token_type {
    tok_eof,
    tok_def,
    tok_extern,
    tok_identifier,
    tok_number,
    tok_dbl,
    tok_string,
    tok_punc,
};

union value_holder {
    char *string;
    double dbl;
    long num;
};

struct token {
    enum token_type type;
    union value_holder value;
    struct token *next_token;
};

#endif
