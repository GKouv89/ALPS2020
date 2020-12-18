#ifndef TF_H
#define TF_H

#include <stdio.h>
#include <stdlib.h>

#include "../BOW/vectorOps.h"
#include "../BOW/bow.h"
#include "idfVectorOps.h"

#ifndef TFVECTORS
    #define TFVECTORS 5
#endif

typedef struct textFreq{
    Vector *wordVec;
    IDFVector *vectors[VECTORS];
} tf;

void create_tf(tf **, int);
void compute_tf_idf(BoW *, tf *, IDFVector *);
void destroy_tf(tf **);
#endif