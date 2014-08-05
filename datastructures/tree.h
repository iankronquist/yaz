#include <stdlib.h>
#include <stdio.h>

struct treenode {
    char* symbol;
    size_t num_children;
    size_t child_slots;
    struct treenode** children;
};

typedef struct treenode tree;

tree* tree_new(char* root_symbol);
void tree_delete(tree*);
void tree_walk(tree*, void func(struct treenode*));
tree* tree_add_child(tree* parent, char* token);
void tree_print(tree*);
void treenode_print(struct treenode* node);
