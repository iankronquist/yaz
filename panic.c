#include "panic.h"
#include <stdlib.h>
#include <stdio.h>

void panic(const char* message, char* context) {
    fprintf(stderr, "Error: %s %s", message, context);
    exit(EXIT_FAILURE);
}
