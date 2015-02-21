#include "panic.h"
#include <stdlib.h>
#include <stdio.h>

void panic(const char* message, char* context) {
    fprintf(stderr, "Error: %s %s", message, context);
    exit(EXIT_FAILURE);
}

void parse_error(char *expected, char *got, char *context) {
    printf("Expected %s, got %s near %s", expected, got, context);
    exit(EXIT_FAILURE);
}
