#ifndef PANIC_H
#define PANIC_H

void panic(const char* message, char* context);
void parse_error(char *expected, char *got, char *context);

#endif
