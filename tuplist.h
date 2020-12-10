#ifndef TUPLIST_H_
#define TUPLIST_H_
// #include "datatypes.h"

typedef struct tuple{
    char* name;
    char* value;
    struct tuple *next;
} node_tuple;

typedef struct listnode{
    node_tuple *head;
    node_tuple *last;
}tuplelist;

int tuplist_empty(tuplelist *);
node_tuple* tuplist_create_node(char*, char*);
void tuplist_create(tuplelist **, int *); 
void tuplist_insert(tuplelist **, char*, char*);
void tuplist_destroy_node(node_tuple **);
void tuplist_destroy(tuplelist **, int *);
//void tuplist_print(tuplelist *, int *);
void tuplist_print(tuplelist **);
#endif