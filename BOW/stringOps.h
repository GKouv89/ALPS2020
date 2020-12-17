#ifndef STRINGOPS_H
#define STRINGOPS_H

#include "stopwords.h"
#include "dictionary.h"
#include "bow.h"

void bow_it(char *, sw_list *, tree_node **, BoW **, int, Vector *);
void insert_in_structures(sw_list *, tree_node **, BoW **, char *, int, int *, Vector *);
#endif