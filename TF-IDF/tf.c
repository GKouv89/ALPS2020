#include <stdlib.h>
#include <assert.h>

#include "tf.h"

void create_tf(tf **tfarr, int words){
    *tfarr = malloc(sizeof(tf));
    create_vector(&(*tfarr)->wordVec);
    
    while((*tfarr)->wordVec->capacity < words){
        (*tfarr)->wordVec->capacity *= 2;
    }
    int *temp = realloc((*tfarr)->wordVec->elements, (*tfarr)->wordVec->capacity*sizeof(int));
    assert(temp != NULL);
    (*tfarr)->wordVec->elements = temp;
    for(int i = 0; i < words; i++){
        (*tfarr)->wordVec->elements[i] = i;
    }
    (*tfarr)->wordVec->size = words;    
    
    double *tempd;
    for(int i = 0; i < TFVECTORS; i++){
        create_idf_vector(&(*tfarr)->vectors[i]);
        (*tfarr)->vectors[i]->capacity = (*tfarr)->wordVec->capacity;
        tempd = realloc((*tfarr)->vectors[i]->elements, (*tfarr)->vectors[i]->capacity*sizeof(double));
        assert(tempd != NULL);
        (*tfarr)->vectors[i]->elements = tempd;
    }
}

void compute_tf_idf(BoW *bag, tf *tfarr, IDFVector *idf_vec){
    for(int i = 0; i < TFVECTORS; i++){
        for(int j = 0; j < bag->vectors[i]->size; j++){
            tfarr->vectors[i]->elements[j] = bag->vectors[i+1]->elements[j];
            tfarr->vectors[i]->elements[j] /= bag->vectors[i+1]->word_count;
            tfarr->vectors[i]->elements[j] *= idf_vec->elements[j];
        }
        tfarr->vectors[i]->size = bag->vectors[i]->size;
    }
}

void destroy_tf(tf **tfarr){
    destroy_vector(&(*tfarr)->wordVec);
    (*tfarr)->wordVec = NULL;
    for(int i = 0; i < TFVECTORS; i++){
        destroy_idf_vector(&(*tfarr)->vectors[i]);
        (*tfarr)->vectors[i] = NULL;
    }
    free(*tfarr);
    *tfarr = NULL;
}
