#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "idfVectorOps.h"

void create_idf_vector(IDFVector **vec){
    *vec = malloc(sizeof(IDFVector));
    (*vec)->name = NULL;
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
  double *temp_d = realloc(temp->elements, temp->capacity*sizeof(double));
  assert(temp_d != NULL);
  temp->elements = temp_d;
  // for(int i = 0; i < size; i++){
    // temp->elements[i] = vec->elements[i];
  // }
  memcpy(temp->elements, vec->elements, temp->capacity*sizeof(double));
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

IDFVector* concatenate_idf_vectors(IDFVector *vec1, IDFVector *vec2){
  IDFVector *new_vector = malloc(sizeof(IDFVector));
  new_vector->size = vec1->size + vec2->size;
  new_vector->capacity = vec1->capacity + vec2->capacity;
  assert(new_vector->size == new_vector->capacity);
  new_vector->elements = calloc(new_vector->capacity, sizeof(double));
  for(int i = 0; i < vec1->size; i++){
    new_vector->elements[i] = vec1->elements[i];
  }
  for(int i = 0; i < vec2->size; i++){
    new_vector->elements[vec1->size + i] = vec2->elements[i];
  }
  return new_vector;
}

void destroy_idf_vector(IDFVector **vec){
    free((*vec)->name);
    free((*vec)->elements);
    free(*vec);
    *vec = NULL;
}