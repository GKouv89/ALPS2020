#ifndef STRINGOPS_H
#define STRINGOPS_H

#include "stopwords.h"
#include "dictionary.h"
#include "bow.h"

void bow_it(char *, sw_list *, tree_node **, BoW **, int);
void insert_in_structures(sw_list *, tree_node **, BoW **, char *, int, int *);
#endif