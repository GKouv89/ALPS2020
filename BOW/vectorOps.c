#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "vectorOps.h"

void create_vector(Vector **vec){
    *vec = malloc(sizeof(Vector));
    (*vec)->name = NULL;
    (*vec)->word_count = 0;
    (*vec)->size = 0;
    (*vec)->capacity = CAPACITY;
    (*vec)->elements = calloc((*vec)->capacity, sizeof(int));
}

void name_vector(Vector *vec, char *name){
    vec->name = malloc(strlen(name) + 1);
    strcpy(vec->name, name);
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

void update_element(Vector *vec, int pos, int *idf_incr){ // This one indicates that we stumbled upon a word that's already in the bag
    // printf("updating element in pos = %d\n", pos);
    // printf("curr elem value = %d\n", vec->elements[pos]);
    (vec->elements[pos])++;
    if(vec->elements[pos] == 1){
        *idf_incr = 1;
        vec->word_count++;
    }else{
        *idf_incr = 0;
    }
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
    free((*vec)->name);
    (*vec)->name = NULL;
    free(*vec);
    *vec = NULL;
}