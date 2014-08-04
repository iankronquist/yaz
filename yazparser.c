#include "yazparser.h"

const char* USEAGE = "Useage:\nyaz filename\n";
static void _error(char* message, char* snippet);
static void _parseHelper(tree* parent, char* sep);


/** Entry point for the parser. TODO: Move to own file
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


/** Open the provided `fileName` and parse it line by line. 
    TODO: pass generated asts to an interpreter. Store functions in a table
    to be reused later.
*/
void parseFile(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        _error("Error: couldn't open file ", fileName);
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


/** Return an abstract syntax tree representing the statement in `line`.
    Uses recursive `parseHelper` to add children.
*/
tree* parseStatement(char* line) {
    if(line[0] == '\n' || (line[0] == '/' && line[1] == '/'))
        return NULL;
    tree* astRoot = malloc(sizeof(struct treenode));
    astRoot->num_children = 0;
    astRoot->children = NULL;
    char* sep = " ";
    char* word;
    word = strtok(line, sep);
    word = strtok(NULL, sep);
    astRoot->symbol = word;
    _parseHelper(astRoot, sep);
    return astRoot;
}


/** Recursive helper to add children to the `parent` tree and add subtrees.
    `parent` is the tree to attach childrent to. `sep` is the separator used by
    strtok to split tokens. It is a " ".

*/
static void _parseHelper(tree* parent, char* sep) {
    char* word = strtok(NULL, sep);
    // If the word it NULL then we've run out of words to process
    if (word == NULL) {
        return;
    } else if (word[0] == '(') {
        // Get the verb for this sub tree
        char* nextWord = strtok(NULL, sep);
        // Add the verb to the tree and get the root of the subtree
        // that is the structure represented by the verb
        tree* sub_tree = tree_add_child(parent, nextWord);
        // Continue parsing for the sub tree
        _parseHelper(sub_tree, sep);
        // When that function returns there was either a newline or a closing
        // paren. In the latter case we need to continue parsing the higher
        // part of the tree
        _parseHelper(parent, sep);
        return;
    } else if (word[0] == ')') {
        // We've finished parsing this subtree
        return;
    } else {
        // 
        tree_add_child(parent, word);
        _parseHelper(parent, sep);
        return;
    }
    assert(0);
}


/** Report parse time errors. Calls exit and terminates the program.
    `message` is the message to display describing the error. `snippet` is
    possibly the snippet of code where the error occurred.
*/
static void _error(char* message, char* snippet) {
    printf("%s %s\n", message, snippet);
    exit(3);
}
