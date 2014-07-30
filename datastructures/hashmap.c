#include "hashmap.h"

struct _hashnode** _hashtable_init(size_t size)
{
    struct _hashnode** table = malloc(size * sizeof(struct _hashnode*));
    memset(table, 0, size * sizeof(struct _hashnode*));
    return table;
}

hashmap* hashmap_new()
{
    hashmap* map = malloc(sizeof(hashmap));
    //16 seems like a good initial map size
    map->_buckets = _hashtable_init(16);
    memset(map->_buckets, 0, sizeof(struct _hashnode*) * 16);
    map->numElems = 0;
    map->numBuckets = 16;
    return map;
}

void hashmap_delete(hashmap* map)
{
    _hashnodetable_delete(map->_buckets, map->numBuckets);
    free(map);
}

void _hashnodetable_delete(struct _hashnode** nodetable, size_t tableSize)
{
    struct _hashnode* node;
    struct _hashnode* tmpnode;
    for(size_t i = 0; i < tableSize; i++)
    {
        node = nodetable[i];
        while(node != NULL)
        {
            free(node->key);
            free(node->val);
            tmpnode = node;
            node = node->next;
            free(node);
            nodetable[i] = NULL;
        }
    }
    free(nodetable);
}

void hashmap_addVal(hashmap* map, char* key, void* val)
{
    //if the max load has been exceeded resize the map to double the capacity
    if(_hashmap_load(map) >= HASHMAPMAXLOAD)
    {
        _hashmap_resize(map, hashmap_numElems(map) * 2);
    }
    struct _hashnode* newnode = malloc(sizeof(struct _hashnode));
    newnode->next = NULL;
    //The key provided may not be on the heap and may be freed elsewhere
    //FIXME I should use strnlen
    char* newKey = malloc(sizeof(char*)*strlen(key));
    //FIXME I should use strncpy
    strcpy(newKey, key);
    newnode->val = val;
    newnode->key = newKey;
    size_t index = _hash(map->numBuckets, key);
    _hashnodetable_add(map->_buckets, newnode, index);
    map->numElems++;
}

void _hashnodetable_add(struct _hashnode** nodetable,
    struct _hashnode* newNode, size_t index)
{
    //Initialize the new node
    struct _hashnode* node = nodetable[index];
    struct _hashnode* prevnode = node;
    while(node != NULL)
    {
        prevnode = node;
        node = node->next;
    }
    if(prevnode != NULL)
        prevnode->next = newNode;
    else
        nodetable[index] = newNode;
}

bool hashmap_hasKey(hashmap* map, char* key)
{
    size_t index = _hash(map->numBuckets, key);
    struct _hashnode* node = map->_buckets[index];
    struct _hashnode* prevnode = node;
    while(node != NULL)
    {
        //FIXME should use strncpy
        if(strcpy(key, node->key) == 0)
        {
            return true;
        }
        prevnode = node;
        node = node->next;
    }
    return false;
}

void hashmap_removeKey(hashmap* map, char* key)
{
    size_t index = _hash(map->numBuckets, key);
    struct _hashnode* node = map->_buckets[index];
    struct _hashnode* prevnode = node;
    while(node != NULL)
    {
        //FIXME should use strncpy
        if(strcpy(key, node->key) == 0)
        {
            free(node->key);
            free(node->val);
            prevnode->next = node->next;
            free(node);
            return;
        }
        prevnode = node;
        node = node->next;
    }
    return;
}

void* hashmap_getVal(hashmap* map, char* key)
{
    size_t index = _hash(map->numBuckets, key);
    struct _hashnode* node = map->_buckets[index];
    struct _hashnode* prevnode = node;
    while(node != NULL)
    {
        //FIXME should use strncpy
        if(strcpy(key, node->key) == 0)
        {
            return node->val;
        }
        prevnode = node;
        node = node->next;
    }
    return NULL;
}

void hashmap_setVal(hashmap* map, char* key, void* val)
{
    size_t index = _hash(map->numBuckets, key);
    struct _hashnode* node = map->_buckets[index];
    struct _hashnode* prevnode = node;
    while(node != NULL)
    {
        //FIXME should use strncpy
        if(strcpy(key, node->key) == 0)
        {
            node->val = val;
            return;
        }
        prevnode = node;
        node = node->next;
    }
    return;
}

size_t hashmap_numElems(hashmap* map)
{
    return map->numElems;
}


//TODO: Is this a reasonable hashing function? Look at examples in
//python or the kernel for reference
size_t _hash(size_t mapsize, char* key)
{
    size_t index = 0;
    //FIXME should use strnlen
    for(int i = 0; i < strlen(key); i++)
    {
        index += key[i];
    }
    index %= mapsize;
    return index;
}

float _hashmap_load(hashmap* map)
{
    return (float)map->numElems/(float)map->numBuckets;
}

void _hashmap_resize(hashmap* map, size_t newSize)
{
    struct _hashnode** newtable = _hashtable_init(newSize);
    struct _hashnode* node;
    for(size_t i = 0; i < map->numBuckets; i++)
    {
        node = map->_buckets[i];
        while(node != NULL)
        {
            size_t index = _hash(map->numBuckets, node->key);
            _hashnodetable_add(newtable, node, index);
        }
    }
    _hashnodetable_delete(map->_buckets, map->numBuckets);
    map->_buckets = newtable;
}
