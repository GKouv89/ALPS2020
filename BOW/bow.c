#include <stdlib.h>

#include "bow.h"

void create_bow(BoW **bag){
    *bag = malloc(sizeof(BoW));
    for(int i = 0; i < VECTORS; i++){
        create_vector(&(*bag)->vectors[i]);
    }
}

void new_text_file(BoW *bag, int pos, char *name){
    name_vector(bag->vectors[pos], name);
}

void new_word_in_bag(BoW *bag, int vector_pos, IDFVector *idf_vec){
    /* We stumbled upon a new word
        First vector updated 
        If first vector is resized, we resize all other vectors, too 
    */
    int resizing;
    new_word(bag->vectors[0], &resizing);
    
    for(int i = 1; i < VECTORS; i++){
        bag->vectors[i]->size++;
        if(resizing){
            resize_vector(bag->vectors[i]);
        }
    }
    int idf_incr;
    update_element(bag->vectors[vector_pos], bag->vectors[0]->size - 1, &idf_incr);

    idf_vec->size++;
    if(resizing){
        resize_idf_vector(idf_vec);
    }
    if(idf_incr){
        update_idf_vector(idf_vec, idf_vec->size - 1);
    }
}

void old_word(BoW *bag, int vector_pos, int word_no, IDFVector *idf_vec){
    int idf_incr;
    update_element(bag->vectors[vector_pos], word_no, &idf_incr);
    if(idf_incr){
        update_idf_vector(idf_vec, word_no);
    }
}

void destroy_bow(BoW **bag){
    for(int i = 0; i < VECTORS; i++){
        destroy_vector(&(*bag)->vectors[i]);
        (*bag)->vectors[i] = NULL;
    }
    free(*bag);
    *bag = NULL;
}
