#ifndef STRINGOPS_H
#define STRINGOPS_H

#include "stopwords.h"
#include "dictionary.h"
#include "bow.h"

void bow_it(char *, sw_list *, tree_node **, BoW **, int);

#endif