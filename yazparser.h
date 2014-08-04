#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "datastructures/hashmap.h"
#include "datastructures/tree.h"

hashmap* symbol_table;
hashmap* function_table;
/** NOT IMPLEMENTED */
void execute_statement(tree* statement);
/** NOT IMPLEMENTED */
void execute_function(void* arguments, size_t num_args, char* function_symbol);

void parseFile(char* fileName);
tree* parseStatement(char* line);


struct function {
    size_t numargs;
    char** argnames;
};

struct variable {
    int type;
    size_t size;
};

union tokenVal {
    struct function func;
    struct variable var;
};

struct token {
    bool isFunction;
    char* name;
    union tokenVal;
};
