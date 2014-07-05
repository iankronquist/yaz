#include <stdio.h>
#include "hashmap.h"

void test_hashmap_new();
void test_hashmap_addVal();
#define EXPECT_EQ(a, b) printf("Test %s == %s ", (#a), (#b));\
if ((a) == (b)) {\
    puts(" passed");\
}\
else {\
    puts("FAILED");\
}

#define EXPECT_NEQ(a, b) printf("Test %s != %s ", (#a), (#b));\
if ((a) != (b)) {\
    puts("passed");\
}\
else {\
    puts("FAILED");\
}

int main()
{
    test_hashmap_new();
    test_hashmap_addVal();
    return 0;
}

void test_hashmap_new()
{
    hashmap* map = hashmap_new();
    EXPECT_EQ(hashmap_numElems(map), 0);    
    for(size_t i = 0; i < map->numBuckets; i++)
    {
        EXPECT_EQ(map->_buckets[i], 0);    
    }
}

void test_hashmap_addVal()
{
    hashmap* map = hashmap_new();
    hashmap_addVal(map, "a", 0);
    struct _hashnode* first = map->_buckets[1];
    EXPECT_EQ(strcmp(first->key, "a"), 0);
    EXPECT_EQ(first->val, 0);
    EXPECT_EQ(first->next, NULL);
    EXPECT_EQ(map->numElems, 1);
}
