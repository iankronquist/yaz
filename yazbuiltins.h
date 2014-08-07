#ifndef YAZBUILTINS_H
#define YAZBUILTINS_H
#include "yaztypes.h"

struct base_type* token_to_type(char* token);
// NOT IMPLEMENTED
char* type_to_str(struct base_type*);

#endif
