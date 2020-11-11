#ifndef __DATATYPES_H__
#define __DATATYPES_H__

// Data Types
// Dataset: A list of all json specs that will be parsed
// spec: Each spec contains id (site//id), info_list
// info_list: list of tuples "name, value" (values is a string, not a number)
// info_tuple: item of above list

typedef struct tuple{
    char* name;
    char* value;
} info_tuple;

// will have head
// double connected
// gets initialized to one node containing page title

typedef struct node{
    char* id; // this will be of the format folder_name//file_numeric_id
    info_tuple content;
    struct node *previous; // previous in hash bucket overflow list
    struct node *next; // next in hash bucket overflow list
    struct node *parent; // WILL BE USED FOR DISJOINT SET OPERATIONS - FINDING OF CLIQUES
    struct node *next_in_clique; // WILL BE USED FOR CLIQUE PRINTING
    int amount; //If disjoint set root, amount of nodes in set. If not, -1. Initialized as 1 because,
    // at the very beginning, each node is a disjoint set of its own.
} list_node;

typedef struct list{
    list_node *front;
    list_node *rear;
} info_list;

typedef struct json{
    char* id;
    info_list information;
} spec;

void list_create(info_list**);
list_node* create_node(char *);
void list_add(info_list*, list_node*);
void list_print(info_list*);
void delete_node(list_node**);
void destroy_list(info_list**);
#endif
 

