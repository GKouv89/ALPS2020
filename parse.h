#ifndef __PARSE_H__
#define __PARSE_H__

#include "hashmap.h"
#include "set.h"
#include "datatypes.h"
#include "tuplist.h"
#include "negcl.h"

#include "BOW/stopwords.h"
#include "BOW/stringOps.h"
#include "BOW/bow.h"
#include "BOW/vectorOps.h"
#include "TF-IDF/idfVectorOps.h"

#ifndef DATASET
    #define DATASET "sigmod_large_labelled_dataset.csv"
#endif

void parser(hash_map*, sw_list *, BoW *, tree_node **, IDFVector *);
void csvparser(hash_map*, clique_list*);
#endif
