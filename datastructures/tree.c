#include "tree.h"

tree* tree_new()
{
    tree* newtree = (tree*)malloc(sizeof(struct treenode*)); 
    tree->num_children = 0;
}

void tree_delete(tree* node)
{
    for(size_t i = 0; i < node->num_children; i++)
    {
        tree_delete(node->children[i]);
    }
    free(node);
}

void tree_walk(tree* node, void func(struct treenode*))
{
    for(size_t i = 0; i < node->num_children; i++)
    {
        tree_delete(node->children[i]);
    }
    func(node);  
}
