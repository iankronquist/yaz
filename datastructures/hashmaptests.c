#include <stdio.h>
#include "hashmap.h"

void test_hashmap_new();
void test_hashmap_delete();
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
