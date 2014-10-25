#ifndef TESTS_H
#define TESTS_H
#include <stdio.h>

#define RED "\x1B[0;31m"
#define GREEN "\x1B[0;32m"
#define NOCOLOR "\x1B[0m"

#define EXPECT_EQ(a, b) printf("Test %s == %s ", (#a), (#b));\
if ((a) == (b)) {\
    printf(" %sPassed%s\n", GREEN, NOCOLOR);\
}\
else {\
    printf(" %sFailed%s\n", RED, NOCOLOR);\
}

#define EXPECT_NEQ(a, b) printf("Test %s != %s ", (#a), (#b));\
if ((a) != (b)) {\
    printf(" %sPassed%s\n", GREEN, NOCOLOR);\
}\
else {\
    printf(" %sFailed%s\n", RED, NOCOLOR);\
}

#define EXPECT_CONTAINS(value, sequence, range)\
printf("Test %s in %s ", (#value), (#sequence));\
int passed = 0;\
for (size_t i = 0; i < range; i++) {\
    if (sequence[i] == value) {\
        passed = 1;\
        printf("%sPassed%s\n", GREEN, NOCOLOR);\
    }\
}\
if (!passed) {\
    printf("%sFailed%s\n", RED, NOCOLOR);\
}

#define EXPECT_NCONTAINS(value, sequence, range)\
printf("Test %s in %s ", (#value), (#sequence));\
int failed = 0;\
for (size_t i = 0; i < range; i++) {\
    if (sequence[i] == value) {\
        failed = 1;\
        printf("%sFailed%s\n", RED, NOCOLOR);\
    }\
}\
if (!failed) {\
    printf("%sPassed%s\n", GREEN, NOCOLOR);\
}

#endif
