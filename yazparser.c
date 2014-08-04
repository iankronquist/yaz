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
    return parseHelper(astRoot, NULL, line, sep);
}

tree* parseHelper(tree* parent, tree* grandParent, char* line, char* sep) {
    char* word = strtok(NULL, sep);
    if (word == NULL) {
        return NULL;
    }
    if (word[0] == '(') {
        char* nextWord = strtok(NULL, sep);
        tree* ggp = grandParent;
        grandParent = parent;
        parent = tree_add_child(parent, nextWord);
        parseHelper(parent, grandParent, line, sep);
        parseHelper(grandParent, ggp, line, sep);
        return parent;
    } else if (word[0] == ')') {
        return parent;
    } else {
        // word in table etc. etc.
        tree_add_child(parent, word);
        parseHelper(parent, grandParent, line, sep);
        return parent;
    }
    assert(0);
    //silence errors
    return NULL;
}

void error(char* message, char* snippet) {
    printf("%s %s\n", message, snippet);
    exit(3);
}
