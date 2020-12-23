#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "tf.h"

void create_tf(tf **tfarr, int words){
    *tfarr = malloc(sizeof(tf));
    create_vector(&(*tfarr)->wordVec);
    
    // while((*tfarr)->wordVec->capacity < words){
        // (*tfarr)->wordVec->capacity *= 2;
    // }
    (*tfarr)->wordVec->capacity = words;
    int *temp = realloc((*tfarr)->wordVec->elements, (*tfarr)->wordVec->capacity*sizeof(int));
    assert(temp != NULL);
    (*tfarr)->wordVec->elements = temp;
    for(int i = 0; i < words; i++){
        (*tfarr)->wordVec->elements[i] = i;
    }
    (*tfarr)->wordVec->size = words;    

    for(int i = 0; i < TFVECTORS; i++){
      (*tfarr)->vectors[i] = malloc(sizeof(IDFVector));
      (*tfarr)->vectors[i]->size = words;
      (*tfarr)->vectors[i]->capacity = words;
      (*tfarr)->vectors[i]->elements = calloc((*tfarr)->vectors[i]->capacity, sizeof(double));
    }
}

IDFVector* compute_tf_idf(BoW *bag, tf *tfarr, IDFVector *idf_vec){
    IDFVector *important_words;
    create_idf_vector(&important_words);
    while(important_words->capacity < idf_vec->size){
        important_words->capacity *= 2;
    }

    double *temp = realloc(important_words->elements, important_words->capacity*sizeof(double));
    assert(temp != NULL);
    important_words->elements = temp;
    memset(important_words->elements, 0, important_words->capacity*sizeof(double));
    
    for(int i = 0; i < TFVECTORS; i++){
        for(int j = 0; j < bag->vectors[i]->size; j++){
            tfarr->vectors[i]->elements[j] = bag->vectors[i+1]->elements[j];
            tfarr->vectors[i]->elements[j] /= bag->vectors[i+1]->word_count;
            tfarr->vectors[i]->elements[j] *= idf_vec->elements[j];
            
            important_words->elements[j] += tfarr->vectors[i]->elements[j];
        }
        tfarr->vectors[i]->size = bag->vectors[i]->size;
    }
    important_words->size = tfarr->wordVec->size;
    for(int i = 0; i < important_words->size; i++){
        important_words->elements[i] /= FILES;
    }
    return important_words;
}

void sort_avg_tf_idf(Vector *wordVec, IDFVector* important_words, int first, int last){
  int i, j, pivot, temp_i;
  double temp_d;
  
  if(first < last){
    pivot = first;
    i = first;
    j = last;
    
    while(i < j){
      while(important_words->elements[i] > important_words->elements[pivot]){
        i++;
      }
      while(important_words->elements[j] <= important_words->elements[pivot] && j > first){
        j--;
      }
      if(i < j){
        temp_d = important_words->elements[i];
        important_words->elements[i] = important_words->elements[j];
        important_words->elements[j] = temp_d;
      
        temp_i = wordVec->elements[i];
        wordVec->elements[i] = wordVec->elements[j];
        wordVec->elements[j] = temp_i;
      }
    }
    
    temp_d = important_words->elements[pivot];
    important_words->elements[pivot] = important_words->elements[j];
    important_words->elements[j] = temp_d;
    
    temp_i = wordVec->elements[pivot];
    wordVec->elements[pivot] = wordVec->elements[j];
    wordVec->elements[j] = temp_i;
    
    sort_avg_tf_idf(wordVec, important_words, first, j-1);
    sort_avg_tf_idf(wordVec, important_words, j+1, last);
  }
}


void size_down_tf_idf(tf *tfarr, tf *tfarr_new, Vector *wordVec/*, IDFVector *important_words*/){
  int position;
  for(int i = 0; i < wordVec->size; i++){
    position = wordVec->elements[i];
    tfarr_new->wordVec->elements[i] = tfarr->wordVec->elements[position];
    for(int j = 0; j < TFVECTORS; j++){
      tfarr_new->vectors[j]->elements[i] = tfarr->vectors[j]->elements[position];
    }
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
