#include "../datastructures/tree.h"
#include "tests.h"

void test_tree_creation_and_deletion();
void test_tree_delete();
void test_tree_walk();
void test_tree_print();

int main()
{
    test_tree_creation_and_deletion();

}

void test_tree_creation_and_deletion() {
    tree* t = tree_new();
    EXPECT_EQ(t->num_children, 0);
    tree* parent = t;
    char* test_strings[] = { "a", "b", "c", "d", "e" };
    for (size_t i = 0; i < 5; i++) {
        parent = tree_add_child(parent, test_strings[i]);
    }
    tree_delete(t);
}

void test_tree_walk() {

}
