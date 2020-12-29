#ifndef NEGCL_H
#define NEGCL_H

#include "datatypes.h"

// currently negative relationship cliques are created
// with a rerun of shop files but this time focusing on the 0 pairs

typedef struct ngnode{
    struct node *neg_clique; 
    // representative of each final positive relation clique will be here
    struct ngnode *next_in_negclique;
    int printed;
}neg_node; //nodes for negative relationship cliques

typedef struct nglist{
        neg_node *front;
        neg_node *rear;
}neg_list;

void neglist_create(struct node**);
neg_node* create_negnode(struct node *);
void neglist_add(struct node*, struct node*);
void neglist_print(struct node*, FILE*);
void delete_negnode(neg_node**);
void destroy_neglist(struct node**);
#endif