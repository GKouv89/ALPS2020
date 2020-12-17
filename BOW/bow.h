#ifndef BOW_H
#define BOW_H

#include <stdio.h>
#include <stdlib.h>

#include "vectorOps.h"


#ifndef VECTORS
    #define VECTORS 5
#endif

typedef struct bag_of_words{
    Vector *vectors[VECTORS];
} BoW;

void create_bow(BoW **);
void new_word_in_bag(BoW *, int, Vector *);
void old_word(BoW *, int, int, Vector *);
void destroy_bow(BoW **);
#endif