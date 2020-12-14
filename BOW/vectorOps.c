#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vectorOps.h"

void create_vector(Vector **vec){
    *vec = malloc(sizeof(Vector));
    (*vec)->size = 0;
    (*vec)->capacity = CAPACITY;
    (*vec)->elements = calloc((*vec)->capacity, sizeof(int));
}

void new_word(Vector *vec, int *resizing){
    /* 
        Added a new word to bow. This updates the 'top' vector, that contains the number of the word
        that a search in the dictionary will return. If this is resized, we resize all other vector of
        the bag, too.
    */
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
    vec->elements[pos] = vec->size; // The number of the word (numbering starts from 0)
    vec->size++;
}

void update_element(Vector *vec, int pos){ // This one indicates that we stumbled upon a word that's already in the bag
    // printf("updating element in pos = %d\n", pos);
    // printf("curr elem value = %d\n", vec->elements[pos]);
    (vec->elements[pos])++;
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