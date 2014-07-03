#include "hashmap.h"

hashmap* hashmap_new()
{
    hashmap* map = (hashmap*)malloc(sizeof(hashmap));
    //16 seems like a good initial map size
    map->buckets = (struct _hashnode**)malloc(sizeof(struct hashnode*) * 16);
    memset(map->buckets, 0, sizeof(struct hashnode*) * 16);
    map->numElems = 0;
    map->numBuckets = 16;
    return map;
}

void hashmap_delete(hashmap* map)
{
    struct _hashnode* node;
    struct _hashnode* tmpnode;
    for(size_t i = 0; i < map->numBuckets; i++)
    {
        node = map->_buckets[i];
        while(node != NULL)
        {
            free(node->key);
            free(node->val);
            tmpnode = node;
            node = node->next;
            free(node);
        }
    }
    free(map->_buckets);
    free(map);
}

void hashmap_addVal(hashmap* map, char* key, void* val)
{
    //if the max load has been exceeded resize the map to double the capacity
    if(_hashmap_load(map) >= HASHMAPMAXLOAD)
    {
        _hashmap_resize(hashmap_numElems(map) * 2);
    }
    //Initialize the new node
    struct _hashnode* newnode =
        (struct _hashnode)malloc(sizeof(struct _hashnode));
    newnode->next = NULL;
    //The key provided may not be on the heap and may be freed elsewhere
    //FIXME I should use strnlen
    char* newKey = (char*)malloc(sizeof(char*)*strlen(key));
    //FIXME I should use strncpy
    strcpy(newKey, key);
    newnode->val = val;
    //iterate through the chain
    size_t index = _hash(key);
    struct _hashnode node = map->_buckets[index];
    struct _hashnode prevnode = node;
    while(node != NULL)
    {
        prevnode = node;
        node = node->next;
    }
    prevnode->next = newnode;
    map->numElems++;
}

bool hashmap_hasKey(hashmap* map, char* key)
{
    size_t index = _hash(key);
    struct _hashnode node = map->_buckets[index];
    struct _hashnode prevnode = node;
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
    size_t index = _hash(key);
    struct _hashnode node = map->_buckets[index];
    struct _hashnode prevnode = node;
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
    size_t index = _hash(key);
    struct _hashnode node = map->_buckets[index];
    struct _hashnode prevnode = node;
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
    size_t index = _hash(key);
    struct _hashnode node = map->_buckets[index];
    struct _hashnode prevnode = node;
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
size_t _hash(hashmap* map, char* key)
{
    size_t mapsize = hashmap_numElems(map);
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

hashmap* _hashmap_resize(hashmap* map, size_t newSize)
{
    _hashnode** newtable = _hashtable_init(newSize);
    struct _hashnode* node;
    struct _hashnode* tmpnode;
    for(size_t i = 0; i < map->numBuckets; i++)
    {
        node = map->_buckets[i];
        while(node != NULL)
        {
            
        }
    }
}
