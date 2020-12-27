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

void parser(hash_map*, sw_list *, BoW *, tree_node **, IDFVector *);
void csvparser(hash_map*, clique_list*);
#endif
