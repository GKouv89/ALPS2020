#ifndef LOGREG_H
#define LOGREG_H

#ifndef IMPWORDS
  #define IMPWORDS 1000
#endif

#define COEFF_AMOUNT 2*IMPWORDS+1
#define TRAINING_SET_MEDIUM "ML_Sets/TrainingSet_medium.csv"
#define VALIDATION_SET_MEDIUM "ML_Sets/ValidationSet_medium.csv"
#define TEST_SET_MEDIUM "ML_Sets/TestSet_medium.csv"
#define TRAINING_SET_LARGE "ML_Sets/TrainingSet.csv"
#define VALIDATION_SET_LARGE "ML_Sets/ValidationSet.csv"
#define TEST_SET_LARGE "ML_Sets/TestSet.csv"
#define EPOCHS 5

#include "TF-IDF/idfVectorOps.h"
#include "TF-IDF/tf.h"
#include "hashmap.h"

double coefficients[COEFF_AMOUNT];

void init_coefficients();
void train(hash_map *, tf *);
double sigmoid(double );
double f(IDFVector *);
void update_coefficients(double, double, IDFVector *);
void validate(hash_map *, tf *);
void test(hash_map *, tf *);
#endif