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
    for(size_t i = 0; i < node->num_children; i++)
    {
        func(node->children[i]);
    }
    func(node);  
}


/** Prints the current tree. Prints the symbol of every `treenode`.
    Uses `tree_walk` and `treenode_print`, a static helper function.
*/
void tree_print(tree* ast)
{
    if (ast == NULL) {
        puts("tree is null");
        return;
    }
    for(size_t i = 0; i < ast->num_children; i++)
    {
        tree_print(ast->children[i]);
    }
    printf("%s\n", ast->symbol);
}

/** A static helper function for tree_print.
*/
static void treenode_print(struct treenode* node)
{
    printf("Node: %s\n", node->symbol);
}
