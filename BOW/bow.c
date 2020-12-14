#include <stdlib.h>

#include "bow.h"

void create_bow(BoW **bag){
    *bag = malloc(sizeof(BoW));
    for(int i = 0; i < VECTORS; i++){
        create_vector(&(*bag)->vectors[i]);
    }
}

void new_word_in_bag(BoW *bag, int vector_pos){
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
    update_element(bag->vectors[vector_pos], bag->vectors[0]->size - 1);
}

void old_word(BoW *bag, int vector_pos, int word_no){
    update_element(bag->vectors[vector_pos], word_no);
}

void destroy_bow(BoW **bag){
    for(int i = 0; i < VECTORS; i++){
        destroy_vector(&(*bag)->vectors[i]);
        (*bag)->vectors[i] = NULL;
    }
    free(*bag);
    *bag = NULL;
}
