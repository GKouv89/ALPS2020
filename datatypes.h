#ifndef __DATATYPES_H__
#define __DATATYPES_H__

#include "tuplist.h"
#include "negcl.h"

typedef struct node{
    char* id; // this will be of the format folder_name//file_numeric_id
    tuplelist *content;
    struct node *previous; // previous in hash bucket overflow list
    struct node *next; // next in hash bucket overflow list
    struct node *parent; // WILL BE USED FOR DISJOINT SET OPERATIONS - FINDING OF CLIQUES
    struct node *next_in_clique; // WILL BE USED FOR CLIQUE PRINTING
    int amount; //If disjoint set root, amount of nodes in set. If not, -1. Initialized as 1 because,
    // at the very beginning, each node is a disjoint set of its own.
    //new//
    neg_list *ngl;
    
} list_node;

typedef struct list{
    list_node *front;
    list_node *rear;
} info_list;

void list_create(info_list**);
list_node* create_node(char*);
void list_add(info_list*, list_node*);
void list_print(info_list*);
void delete_node(list_node**);
void destroy_list(info_list**);
#endif
 

