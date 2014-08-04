#include "yazparser.h"

const char* USEAGE = "Useage:\nyaz filename\n";
void read_file_by_lines(char* filename);
void extend_children_list(struct treenode* parent,
        struct treenode* newchild);
struct treenode* add_ast(struct treenode* parent);
void error(char* message, char* snippet);

/*
   Program return values:
 * 0: exited successfully
 * 1: invalid arguments
 * 2: file read error
 * 3: invalid syntax
 */
int main(int argc, char** argv)
{
    if(argc == 1)
    {
        printf("%s", USEAGE);
        return 1;
    }
    parseFile(argv[1]);
}

void parseFile(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        error("Error: couldn't open file ", fileName);
        return;
    }
    char* line = NULL;
    size_t line_capacity = 0;
    ssize_t line_length = 0;
    while ((line_length = getline(&line, &line_capacity, file)) > 0) {
        tree* ast = parseStatement(line);
        tree_print(ast);
        tree_delete(ast);
    }
    fclose(file);
}

tree* parseStatement(char* line) {
    if(line[0] == '\n' || (line[0] == '/' && line[1] == '/'))
        return NULL;
    tree* astRoot = malloc(sizeof(struct treenode));
    astRoot->num_children = 0;
    astRoot->children = NULL;
    char* sep = " ";
    char* word;
    word = strtok(line, sep);
    //assert(word[0] == '(');
    word = strtok(NULL, sep);
    astRoot->symbol = word;
    parseHelper(astRoot, line, sep);
    return astRoot;
}

void parseHelper(tree* parent, char* line, char* sep) {
    char* word = strtok(NULL, sep);
    if (word == NULL) {
        return;
    } else if (word[0] == '(') {
        // Get the verb for this sub tree
        char* nextWord = strtok(NULL, sep);
        // Add the verb to the tree and get the root of the subtree
        // that is the structure represented by the verb
        tree* sub_tree = tree_add_child(parent, nextWord);
        // Continue parsing for the sub tree
        parseHelper(sub_tree, line, sep);
        // When that function returns there was either a newline or a closing
        // paren. In the latter case we need to continue parsing the higher
        // part of the tree
        parseHelper(parent, line, sep);
        return;
    } else if (word[0] == ')') {
        return;
    } else {
        // word in table etc. etc.
        tree_add_child(parent, word);
        parseHelper(parent, line, sep);
        return;
    }
    assert(0);
}

void error(char* message, char* snippet) {
    printf("%s %s\n", message, snippet);
    exit(3);
}
