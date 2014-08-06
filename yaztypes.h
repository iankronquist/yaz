#ifndef YAZTYPES_H
#define YAZTYPES_H

#include <stdbool.h>

/*
The following are the types in yaz:
* int_type        -> c int. Acts, walks, and talks like an int.
* bool_type       -> Acts like a bool should
* string_type     -> Acts like a string should. Has a length.
* array_type      -> An array of one kind of type.
* poly_array_type -> An array of many kinds of types.
* func_type      -> represents a yaz function, which is really a list of 
    yaz functions.
* _list_t         -> provides a unified c interface for _string_type,
    array_type, and poly_array_type
*/


enum type {
    int_t,
    _list_t,
    string_t,
    array_t,
    poly_array_t,
    funct_t,
};


struct base_type {
    enum type;
};


struct bool_type {
    struct base_type type;
    bool value;

};


struct int_type {
    struct base_type;
    int value;
};


static struct _list_t {
    enum type;
    size_t length;
}


struct func_t {
    _list_t list;
    struct funct_t* statements;
};


struct string_type {
    _list_t list;
    char* data;
};


struct array_type {
    _list_t list;
    enum inner_type;
    void* data;
};


struct poly_array_t {
    _list_t list;
    void* data;
};

#endif
