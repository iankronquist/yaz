#include <stdlib.h>
#include <stdio.h>

struct treenode {
    char* symbol;
    size_t num_children;
    struct treenode** children;
};

typedef struct treenode tree;

tree* tree_new();
void tree_delete(tree*);
void tree_walk(tree*, void func(struct treenode*));
void tree_print(tree*);
