#include "tree.h"

/** Recursion helper for accurately formatting the tree. */
void _tree_print_helper(tree* node);

/** Create a new tree with no children.
    The tree's `symbol` will be `root_symbol`.
    *Note:* Does not allocate the tree's `children` member. Those can be
    allocated later
*/
tree* tree_new(char* root_symbol)
{
    tree* newtree = malloc(sizeof(struct treenode)); 
    newtree->symbol = root_symbol;
    // Most functions will have few children
    newtree->child_slots = 4;
    newtree->num_children = 0;
    newtree->children = malloc(newtree->child_slots * sizeof(tree*));
    return newtree;
}


/** Add a new child to `parent`. Returns the new child.
    Note that this will realloc the children array.
*/
tree* tree_add_child(tree* parent, char* symbol) {
    tree* newElem = tree_new(symbol);
    if (parent->child_slots == parent->num_children) {
        parent->child_slots *= 2;
        parent->children = realloc(parent->children, parent->child_slots *
            sizeof(tree*));
    }
    parent->children[parent->num_children] = newElem;
    parent->num_children++;
    return newElem;
}


/** Recursively free `node` and all descendants of the `node`.
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
    free(node->children);
    free(node);
}


/** Apply some function to each node of the tree
    starting with the provided `node`.
*/
void tree_walk(tree* node, void func(struct treenode*))
{
    if (node == NULL) {
        printf("%s", "tree is null");
        return;
    }
    func(node);
    for(size_t i = 0; i < node->num_children; i++)
    {
        tree_walk(node->children[i], func);
    }
}

/** Prints the tree to standard out.
    Prints the symbol of every `treenode`.
    Uses `tree_walk` and `treenode_print`, a static helper function.
*/
void tree_print(tree* node)
{
    if (node == NULL) {
        printf("%s\n", "tree is null");
        return;
    }
    printf("%s", "(");
    _tree_print_helper(node);
    printf("%s", ")\n");
}

void _tree_print_helper(tree* node) {
    printf(" %s ", node->symbol);
    if (node->num_children == 0) {
        return;
    }
    printf("%s", "(");
    for(size_t i = 0; i < node->num_children; i++)
    {
        _tree_print_helper(node->children[i]);
    }
    printf("%s", ")");
}
