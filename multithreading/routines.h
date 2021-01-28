#ifndef ROUTINES_H
#define ROUTINES_H

#include "../hashmap.h"
#include "../TF-IDF/tf.h"
#include "../TF-IDF/idfVectorOps.h"
#include "matchlist.h"
#include "../negcl.h"

int train(hash_map *, tf *, char *, double[], double[]);
double test(hash_map *, tf *, char *, double, double[], int *);
double sigmoid(double);
double f(IDFVector *, double[]);
void update_coefficients(double[], double, double, IDFVector *);
void threshold_tuning(hash_map *, tf *, char *, double[]);
void conflict_resolution(hash_map*, tf*, double, char *, double[]);
#endif