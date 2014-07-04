#include <stdlib.h>

struct treenode {
    unsigned int symbol;
    size_t num_children;
    struct treenode** children;
}

typedef struct treenode treenode;

tree* tree_new();
void tree_delete(tree*);
void tree_walk(tree*, void func(struct treenode*));
