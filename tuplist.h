#ifndef TUPLIST_H_
#define TUPLIST_H_
#include "datatypes.h"

int list_empty(tuplelist *);
node_tuple* list_create_node(char*, char*);
void list_create(tuplelist **, int *); 
void list_insert(tuplelist **, char*, char*);
void list_destroy_node(node_tuple **);
void list_destroy(tuplelist **, int *);
void list_print(tuplelist *, int *);
#endif