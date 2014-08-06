#include <stdlib.h>
#include <stdio.h>
#include "../yaztypes.h"

struct treenode {
    struct base_type* type;
    size_t num_children;
    size_t child_slots;
    struct treenode** children;
};

typedef struct treenode tree;

tree* tree_new(struct base_type* root_obj);
void tree_delete(tree*);
void tree_walk(tree*, void func(struct treenode*));
tree* tree_add_child(tree* parent, struct base_type* child_obj);
void tree_print(tree*);
void treenode_print(struct treenode* node);
