#ifndef STRINGOPS_H
#define STRINGOPS_H

#include "stopwords.h"
#include "dictionary.h"
#include "bow.h"
#include "../TF-IDF/idfVectorOps.h"

void bow_it(char *, sw_list *, tree_node **, BoW **, int, IDFVector *);
void insert_in_structures(sw_list *, tree_node **, BoW **, char *, int, int *, IDFVector *);
#endif