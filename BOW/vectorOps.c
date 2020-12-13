#include <stdlib.h>
#include <assert.h>

#include "vectorOps.h"

void create_vector(Vector **vec){
    *vec = malloc(sizeof(Vector));
    (*vec)->size = 0;
    (*vec)->capacity = 100;
    (*vec)->elements = calloc((*vec)->capacity, sizeof(int));
}

void insert_element(Vector *vec, int *resizing){ // This function indicates we stumbled upon a new word
    int pos = vec->size;
    if(vec->size == vec->capacity){
        *resizing = 1;
        vec->capacity = 2 * vec->capacity;
        int *temp = realloc(vec->elements, vec->capacity*sizeof(int));
        assert(temp != NULL);
        vec->elements = temp;
        for(int i = vec->size; i < vec->capacity; i++){
            vec->elements[i] = 0;
        }
    }else{
        *resizing = 0;
    }
    vec->elements[pos] = 1;
    vec->size++;
}

void update_element(Vector *vec, int pos){ // This one indicates that we stumbled upon a word that's already in the bag
    vec->elements[pos]++;
}

void resize_vector(Vector *vec){
    vec->capacity = 2 * vec->capacity;
    int *temp = realloc(vec->elements, vec->capacity*sizeof(int));
    assert(temp != NULL);
    vec->elements = temp;
    for(int i = vec->size; i < vec->capacity; i++){
        vec->elements[i] = 0;
    }
}

void destroy_vector(Vector **vec){
    free((*vec)->elements);
    free(*vec);
    *vec = NULL;
}