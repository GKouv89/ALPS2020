#ifndef __SET_H__
#define __SET_H__

#include "datatypes.h"

list_node* find_root(list_node*);
list_node* join_sets(list_node*, list_node*);
void print_clique(int, list_node*);
#endif