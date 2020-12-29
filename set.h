#ifndef __SET_H__
#define __SET_H__

#include "datatypes.h"

typedef struct c_node{
    list_node* representative;
    struct c_node* prev;
    struct c_node* next;
}clique_list_node;

typedef struct c_list{ // This will hold all clique representatives.
    // This way, we won't have to go through every single hash bucket
    // and through every single node, find out whether it is a clique representative,
    // and then print its clique.
    clique_list_node* front;
    clique_list_node* rear;
}clique_list;

void create_clique_list(clique_list**);
void insert_master(clique_list*, list_node*);
void remove_master(clique_list*, list_node*);
list_node* find_root(list_node*);
list_node* join_sets(clique_list*, list_node*, list_node*);
void print_clique(int, list_node*, FILE*);
void print_all_cliques(int, clique_list*);
void delete_clique_node(clique_list_node**);
void destroy_clique_list(clique_list **);
#endif