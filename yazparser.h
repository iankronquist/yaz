#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "datastructures/hashmap.h"
#include "datastructures/tree.h"

hashmap* symbol_table;
hashmap* function_table;
void read_file_by_lines(char* filename);
tree* build_ast(char* line);
void add_token_to_tree(char* token);
void execute_statement(tree* statement);
void execute_function(void* arguments, size_t num_args, char* function_symbol);
void cleanup();
void exit_with_message(char* message);
void parseFile(char* fileName);
tree* parseStatement(char* line);
void addTokenToTree(tree* astParent, char* token, size_t tokenLength);
void addSubTreeToTree(tree* astParent, tree* subTree);
tree* parseStatement(char* line);
tree* parseHelper(tree* parent, tree* grandParent, char* line, char* sep, char* brk);
tree* addChild(tree* parent, char* token);


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
