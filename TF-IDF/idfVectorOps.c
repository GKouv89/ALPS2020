#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "idfVectorOps.h"

void create_idf_vector(IDFVector **vec){
    *vec = malloc(sizeof(IDFVector));
    (*vec)->size = 0;
    (*vec)->capacity = CAPACITY;
    (*vec)->elements = calloc((*vec)->capacity, sizeof(double));
}

void update_idf_vector(IDFVector *vec, int pos){ 
    // We stumbled upon a text that contains word with payload pos
    (vec->elements[pos])++;
}

IDFVector* crop_idf_vector(IDFVector *vec, int size){
  IDFVector *temp;
  create_idf_vector(&temp);
  temp->capacity = size;
  double *temp_d = realloc(temp->elements, temp->capacity*sizeof(int));
  assert(temp_d != NULL);
  temp->elements = temp_d;
  for(int i = 0; i < size; i++){
    temp->elements[i] = vec->elements[i];
  }
  temp->size = size;
  return temp;
}

void resize_idf_vector(IDFVector *vec){
    vec->capacity = 2 * vec->capacity;
    double *temp = realloc(vec->elements, vec->capacity*sizeof(double));
    assert(temp != NULL);
    vec->elements = temp;
    for(int i = vec->size; i < vec->capacity; i++){
        vec->elements[i] = 0;
    }
}

void compute_idf_vals(IDFVector *idf_vec){
    /* Only 'valid' for vector to hold idf values
        Computes IDF value for each word 
        after we counted in how many texts said word appears */
    for(int i = 0; i < idf_vec->size; i++){
        idf_vec->elements[i] = 1/idf_vec->elements[i];
        idf_vec->elements[i] *= FILES;
        idf_vec->elements[i] = log10f(idf_vec->elements[i]);
    }
}

void destroy_idf_vector(IDFVector **vec){
    free((*vec)->elements);
    free(*vec);
    *vec = NULL;
}