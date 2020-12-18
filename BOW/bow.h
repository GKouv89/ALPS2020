#ifndef BOW_H
#define BOW_H

#include <stdio.h>
#include <stdlib.h>

#include "vectorOps.h"
#include "../TF-IDF/idfVectorOps.h"

#ifndef VECTORS
    #define VECTORS 5
#endif

typedef struct bag_of_words{
    Vector *vectors[VECTORS];
} BoW;

void create_bow(BoW **);
void new_text_file(BoW *, int, char *);
void new_word_in_bag(BoW *, int, IDFVector *);
void old_word(BoW *, int, int, IDFVector *);
void destroy_bow(BoW **);
#endif