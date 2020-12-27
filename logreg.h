#ifndef LOGREG_H
#define LOGREG_H

#ifndef IMPWORDS
  #define IMPWORDS 1000
#endif

#define COEFF_AMOUNT 2*IMPWORDS+1
#define TRAINING_SET "TrainingSet.csv"
#define VALIDATION_SET "ValidationSet.csv"
#define EPOCHS 5
#define EPSILON 0.05

#include "TF-IDF/idfVectorOps.h"
#include "TF-IDF/tf.h"
#include "hashmap.h"

double coefficients[COEFF_AMOUNT];

void init_coefficients();
void train(hash_map *, tf *);
double sigmoid(double );
double f(IDFVector *, IDFVector *);
int update_coefficients(double, double, IDFVector *, IDFVector *);
void validate(hash_map *, tf *);
#endif