#ifndef ROUTINES_H
#define ROUTINES_H

#include "../hashmap.h"
#include "../TF-IDF/tf.h"
#include "../TF-IDF/idfVectorOps.h"

int train(hash_map *, tf *, char *, double[], double[]);
double test(hash_map *, tf *, char *, double[], int *);
double sigmoid(double);
double f(IDFVector *, double[]);
void update_coefficients(double[], double, double, IDFVector *);
void conflict_resolution(hash_map*, tf*, double[]);
#endif