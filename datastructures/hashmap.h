#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
struct _hashnode {
    struct _hashnode* next;
    void* val;
    char* key;
};

struct hashmap {
    struct _hashnode** _buckets;
    size_t numBuckets;
    size_t numElems;
};

//TODO: determine whether this is a reasonable number
#define HASHMAPMAXLOAD .75

typedef struct hashmap hashmap;

//Creates a new hashmap
hashmap* hashmap_new();
//Creates a new hashmap of arbitrary size
struct _hashnode** _hashtable_init(size_t size);
//deletes a hashmap
void hashmap_delete(hashmap* map);
//Adds a key value pair to the hashmap
void hashmap_addVal(hashmap* map, char* key, void* val);
//Returns true if the key is in the hashmap, otherwise false
bool hashmap_hasKey(hashmap* map, char* key);
//Removes a key value pair from the hashmap
void hashmap_removeKey(hashmap* map, char* key);
//Given a key, get a value. If the value does not exist, return NULL
void* hashmap_getVal(hashmap* map, char* key);
//Given a key and a new value set the key's value to the new value
//NOTE: The original value will be freed
void hashmap_setVal(hashmap* map, char* key, void* val);
//Returns the number of elements in the hash map
size_t hashmap_numElems(hashmap* map); 
//Return the bucket where the key should reside
size_t _hash(size_t mapsize, char* key);
//Return the load on the hashmap
float _hashmap_load(hashmap* map);
//Resize the hashmap
void _hashmap_resize(hashmap* map, size_t newSize);
void _hashnodetable_add(struct _hashnode** nodetable,
    struct _hashnode* newNode, size_t index);
void _hashnodetable_delete(struct _hashnode** nodetable, size_t tablesize);
#endif
