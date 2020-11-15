#ifndef DATATYPES_H_
#define DATATYPES_H_
// Data Types
// Dataset: A list of all json specs that will be parsed
// spec: Each spec contains id (site//id), info_list
// info_list: list of tuples "name, value" (values is a string, not a number)
// info_tuple: item of above list

typedef struct tuple{
    char* name;
    char* value;
    struct tuple *next;
} node_tuple;

typedef struct listnode{
    node_tuple *head;
    node_tuple *last;
}tuplelist;

// will have head
// double connected
// gets initialized to one node containing page title

typedef struct node{
    char* id;
    tuplelist *content;
    struct node *previous;
    struct node *next;
} list_node;

typedef struct list{
    list_node *front;
} info_list;
 

#endif