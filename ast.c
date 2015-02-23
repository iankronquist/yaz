#include "ast.h"


/*
    Create a new ast node with optional children.
    Pre Conditions:
    * `val` must not be NULL.
    * `num_children` must equal the number of children and may be 0.
    Returns a newly allocated ast node which holds the provided token and
    has the provided children.
*/
struct ast_node* make_node(struct token *val, size_t num_children, ...) {
    struct ast_node *new_node = malloc(sizeof(struct ast_node));
    new_node->val = val;
    new_node->num_children = num_children;
    // pointers are small, so allocating extra space is cheap
    new_node->children_cap = num_children + 2;
    new_node->children = calloc(num_children + 2, sizeof(struct ast_node*));
    va_list children;
    va_start(children, num_children);
    for (size_t i = 0; i < num_children; i++) {
        new_node->children[i] = va_arg(children, struct ast_node*);
    }
    va_end(children);
    return new_node;
}


/*
    Add a child to the provided node.
    Pre Conditions:
    * `node` must not be NULL.
    * `val`, the new child, should not be NULL.
    Post Conditions:
    * `node` has an additional child which is the provided `val`.
*/
void append_child(struct ast_node *node, struct ast_node *val) {
    // double the size of the array of children if the capacity has been
    // reached.
    if (node->num_children == node->children_cap) {
        node->children_cap *= 2;
        node->children = realloc(node->children,
            node->num_children * 2 * sizeof(struct ast_node*));
    }
    node->children[node->num_children] = val;
    node->num_children++;
}


/*
    Recursively free all nodes and values.
    Pre Conditions:
    * `node` must not be NULL.
    * `node` must be on the heap.
    * `node`'s value and children array pointers must be allocated on the heap.
    Post Conditions:
    * `node`'s children are freed.
    * `node`'s children array is freed.
    * `node`'s value is freed.
    * `node` is freed.
*/
void delete_node(struct ast_node *node) {
    for (size_t i = 0; i < node->num_children; i++) {
        delete_node(node->children[i]);
    }
    free(node->children);
    free(node->val);
    free(node);
}


/*
    A post-order map accross all values in the tree.
    Pre Conditions:
    * `node` must not be NULL.
    * `func` is a non-NULL function pointer which takes a pointer to a node and
      returns void.
*/
void ast_post_map(struct ast_node *node, void (*func)(struct ast_node*)) {
    for (size_t i = 0; i < node->num_children; i++) {
        ast_post_map(node->children[i], func);
    }
    func(node);
}


/*
    A post-order map accross all values in the tree.
    Pre Conditions:
    * `node` must not be NULL.
    * `func` is a non-NULL function pointer which takes a pointer to a node and
      returns void.
*/
void ast_pre_map(struct ast_node *node, void (*func)(struct ast_node*)) {
    func(node);
    for (size_t i = 0; i < node->num_children; i++) {
        ast_pre_map(node->children[i], func);
    }
}
