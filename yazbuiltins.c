#include "yaztypes.h"

/** Takes a token, returns the associated value.
    TODO: split into separate functions
    TODO: write tests for this
    Valid values resemble:
        * numbers:
             * must start with a number and be composed of numerals between
                0 and 9
        * bools:
            * be true
            * be false
        * be variables:
            * should exist in symbol_table
        * be strings:
            * should start with "
            * and end with "
            * and contain no spaces 
            * TODO: fix this awful parser bug. Strings aren't a top priority
        * be a open paren (
        * be a close paren )
        * and be nothing else
*/
struct base_type* token_to_type(char* token) {
    // If the token starts with a number
    if (token[0] >= '0' && token[0] <= '9') {
        // Then it should be made into an integer type
        int number = 0;
        for (int i = 0; token[i] != '\0'; i++) {]
            //TODO: make this a real check
            assert(token[i] >= '0' && token[i] <= '9');
            number *= 10;
            number += token[i] - '0';
        }
        struct int_type* integer = malloc(sizeof(struct int_type));
        integer->type->type = int_t;
        integer->value = number;
        return integer;
    } else if (strcmp(token, "true") == 0) {
        struct bool_type* boolean = malloc(sizeof(bool_type));
        boolean->type->type = bool_t;
        boolean->value = true;
        return boolean;
    } else if (strcmp(token, "false") == 0) {
        struct bool_type* boolean = malloc(sizeof(bool_type));
        boolean->type->type = bool_t;
        boolean->value = false;
        return boolean;
    } else if ((token[0] >= 'a' && token[0] <= 'z') || // else if the token is
               (token[0] >= 'A' && token[0] <= 'Z') || // in a-z, A-Z, or is 
               token[0] == '-'                         // either - or _
               || token[0] == '_'
              ) {
        if (struct base_type* type = hashmap_hasKey(symbol_table, token)) {
            return type;
        } else {
            // TODO: show a real error, the symbol hasn't yet been defined
            assert(0);
        }
    } else if (token[0] == '"') { // else if the token starts with a quote
        // then it's a string, and should be made a string type
        struct string_type* string = malloc(sizeof(struct string_type));
        string->list->type = string_t;
        // minus 2 for the opening and closing quotes
        string->list->length = strlen(token) - 2;
        // TODO: make this a real check, not just an assert
        assert(token[string->list->length] == '"');
        char* data = malloc(sizeof(char) * string->list->length);
        string->data = data;
        return string;
    } else if (token[0] == '(') { // this is the end of a statement
        struct base_type* openParen = malloc(sizeof(base_type));
        openParen->type = STMT_BEGIN;
        return openParen;
    } else if (token[0] == ')') { // This is the beginning of a statement
        struct base_type* closeParen = malloc(sizeof(base_type));
        closeParen->type = STMT_END;
        return closeParen;
    } else {
        // Invalid token - bail
        // TODO make this a real check with an error message
        assert(0);
    }
}
