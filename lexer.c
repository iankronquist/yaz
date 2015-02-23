#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "token.h"
#include "token_list.h"
#include "panic.h"

/**
    @brief `get_tok` is a state machine. It parses this string into a list
    of tokens.
    @param The parameter `buffer` is NULL terminated and as long as
    `buffer_len`.
    @return A linked list of tokens.

    The list of possible types of tokens in the token list can be found in
    `token.h`.  It takes a newline terminated string `buffer` and the length of
    this string. 

    The `get_tok` state machine is initialized in the UNKNOWN state. It will
    then iterate through the buffer and enter one of the following states:

    - IDENT if the current character is alphabetic or `_`
    - INT if the current character is numeric
    - STR if the current character is `"`
    - SYMBOL if the current character is anything else, but
      not `#` or ` ` (space).

    If the state is UNKNOWN and current character is `#` a comment has been
    encountered and `get_tok` will stop lexing and return the token list.

    Each state will store the current position in the buffer as `token_begin`
    and will continue iterating through the string until the state's exit
    condition is met or the newline is reached. When the state's exit condition
    is fulfilled a new token is minted and added to the list, and the state is
    reset appropriately.

    The following is the list of exit conditions for each state:

    - INT will exit to DBL if the current character is '.'. It will exit to
      UNKNOWN if the current character is not a digit.
    - DBL will exit to UNKNOWN if the current character is not a digit.
    - STR will exit to UNKNOWN if the current character is `"` and the previous
      character was not `\`.
    - SYMBOL will exit if the current character is alphnumeric, `_`, or `\n`.
*/
struct token_list* get_tok(char* buffer, size_t buffer_len) {
    struct token_list* tk_list = make_token_list();
    char* token_begin = buffer;
    char* token_end = NULL;
    lex_state state = UNKNOWN;
    for (size_t index = 0; index < buffer_len; index++) {
        switch (state) {
            case UNKNOWN:
                token_begin = &buffer[index];
                if (isalpha(buffer[index]) || buffer[index] == '_') {
                    state = IDENT;
                } else if (isdigit(buffer[index])) {
                    state = INT;
                } else if (buffer[index] == '"') {
                    state = STR;
                } else if (buffer[index] == '#') {
                    return tk_list;
                } else if (buffer[index] != ' ') {
                    state = SYMBOL;
                }
                break;
            case IDENT:
                if (!isalnum(buffer[index]) && buffer[index] != '_') {
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_ident(token_begin, token_end));
                    state = UNKNOWN;
                }
                break;
            case INT:
                if (buffer[index] == '.') {
                    state = DBL;
                } else if (!isdigit(buffer[index])) {
                    state = UNKNOWN;
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_int(token_begin, token_end));
                }
                break;
            case DBL:
                if (!isdigit(buffer[index])) {
                    state = UNKNOWN;
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_dbl(token_begin, token_end));
                }
                break;
            case STR:
                if (&buffer[index] == buffer) {
                    panic("Lines can't start with a string", buffer);
                }
                if (buffer[index] == '"' && buffer[index-1] != '\\') {
                    state = UNKNOWN;
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_str(token_begin, token_end));
                }
                break;
            case SYMBOL:
                if (isalnum(buffer[index]) || buffer[index] == '_' ||
                        buffer[index] =='\n') {
                    state = UNKNOWN;
                    token_end = &buffer[index];
                    append_token_list(tk_list, mint_symbol(token_begin, token_end));
                }
                break;
        }
    }
    return tk_list;
}

/**
    @brief Allocate a new token of type `tok_identifier`, `tok_extern`, or
    `tok_def`.

    If the token is of type `tok_identifier` its value will be copied from
    `token_begin` in a new NULL terminated allocation. Otherwise its value will
    be NULL.

    @param 
    The parameters `token_begin` and `tok_end` are non-null pointers to
    a string. `token_end` is greater than `token_end`.

    @return
    Returns newly allocated token. If the new token is of type
    `tok_identifier` its value will be another new allocation.
*/
struct token* mint_ident(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    struct token* tk = malloc(sizeof(struct token));
    if (strncmp(token_begin, "extern", 6) == 0) {
        tk->type = tok_extern;
        tk->value.string = NULL;
        tk->next_token = NULL;
    } else if (strncmp(token_begin, "def", 3) == 0) {
        tk->type = tok_def;
        tk->value.string = NULL;
        tk->next_token = NULL;
    } else {
        size_t len = token_end - token_begin;
        // + 1 for null byte
        char *receptacle = malloc(len + 1);
        tk->type = tok_identifier;
        tk->next_token = NULL;
        // strncpy will not terminate the string witha NULL byte.
        strncpy(receptacle, token_begin, len);
        receptacle[len] = '\0';
        tk->value.string = receptacle;
    }
    return tk;
}

struct token* mint_int(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    struct token* tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    tk->type = tok_number;
    tk->value.num = strtol(token_begin, NULL, 10);
    return tk;
}

struct token* mint_dbl(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    struct token* tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    tk->type = tok_dbl;
    tk->value.dbl = atof(token_begin);
    return tk;
}

struct token* mint_str(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    assert(*token_begin == '"');
    assert(*token_end == '"');
    size_t len = token_end - token_begin;
    struct token* tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    char *receptacle = malloc(len);
    // constants to remove quotes
    strncpy(receptacle, token_begin + 1, len - 1);
    // strncpy will not terminate the string in this case
    receptacle[len-1] = '\0';
    tk->type = tok_string;
    tk->value.string = receptacle;
    return tk;
}

struct token *mint_symbol(char *token_begin, char *token_end) {
    assert(token_end > token_begin);
    size_t len = token_end - token_begin;
    if (len > 2) {
        panic("Too many symbols!", token_begin);
    }
    struct token* tk = malloc(sizeof(struct token));
    tk->next_token = NULL;
    char *receptacle = malloc(3);
    strncpy(receptacle, token_begin, len);
    receptacle[len] = '\0';
    tk->type = tok_punc;
    tk->value.string = receptacle;
    return tk;
}
