#include "tree.h"

static void treenode_print(struct treenode* node);

/** Create a new tree with no children.
    *Note:* Does not allocate the tree's `children` member. Those can be
    allocated later
*/
tree* tree_new()
{
    tree* newtree = malloc(sizeof(struct treenode)); 
    newtree->num_children = 0;
    return newtree;
}


/** Recursively free all descendants of the `node`.
    Note that this will not free the *values* of those nodes - those must be
    freed separately, perhaps using a helper function and `tree_walk`.
*/
void tree_delete(tree* node)
{
    if (node == NULL) {
        return;
    }
    for(size_t i = 0; i < node->num_children; i++)
    {
        tree_delete(node->children[i]);
    }
    free(node);
}

/**
    Apply some function to each node of the tree starting with
    the provided `node`.
*/
void tree_walk(tree* node, void func(struct treenode*))
{
    if (node == NULL) {
        puts("tree is null");
        return;
    }
    func(node);
    for(size_t i = 0; i < node->num_children; i++)
    {
        func(node->children[i]);
    }
}


/** Prints the current tree. Prints the symbol of every `treenode`.
    Uses `tree_walk` and `treenode_print`, a static helper function.
*/
void tree_print(tree* ast)
{
    printf("%s", "(");
    tree_walk(ast, treenode_print);
    printf("%s\n", ")");
}

/** A static helper function for tree_print.
*/
static void treenode_print(struct treenode* node)
{
    printf(" %s ", node->symbol);
}
