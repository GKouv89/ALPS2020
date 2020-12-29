#include "acutest.h"
#include "../BOW/bow.h"
#include "../BOW/vectorOps.h"
#include "../TF-IDF/idfVectorOps.h"
#include "../TF-IDF/tf.h"
#include "../logreg.h"

struct word{
    int word_no;
};

// We're assuming this is the number/position of the word
// as returned by the dictionary
struct word first_sentence[] = {
    [0].word_no = 0, 
    [1].word_no = 1,
    [2].word_no = 2,
    [3].word_no = 3,
    [4].word_no = 4,
    [5].word_no = 5,
    [6].word_no = 6,
    [7].word_no = 7
};

struct word second_sentence[] = {
    [0].word_no = 2,
    [1].word_no = 4,
    [2].word_no = 8,
    [3].word_no = 4,
    [4].word_no = 1
};

void test_createbag(void){
    BoW *bag;
    create_bow(&bag);
    TEST_ASSERT(bag != NULL);
    for(int i = 0; i < VECTORS; i++){
        TEST_ASSERT(bag->vectors[i] != NULL);
        TEST_ASSERT(bag->vectors[i]->elements != NULL);
    }
}

void test_idf(void){
    BoW *bag;
    create_bow(&bag);
    IDFVector *idf_vec;
    create_idf_vector(&idf_vec);
    
    for(int i = 0; i < 8; i++){
        new_text_file(bag, 1, "first_sentence");
        if(first_sentence[i].word_no >= bag->vectors[0]->size){
            new_word_in_bag(bag, 1, idf_vec);
        }else{
            old_word(bag, 1, first_sentence[i].word_no, idf_vec);
        }
    }
    for(int i = 0; i < 5; i++){
        new_text_file(bag, 2, "second_sentence");
        if(second_sentence[i].word_no >= bag->vectors[0]->size){
            new_word_in_bag(bag, 2, idf_vec);
        }else{
            old_word(bag, 2, second_sentence[i].word_no, idf_vec);
        }
    }
    printf("\n");
    for(int i = 0; i < 3; i++){
        if(i == 0){
            printf("WORDS                ");
        }else{
            printf("%s ", bag->vectors[i]->name);            
        }
        for(int j = 0; j < bag->vectors[i]->size; j++){
            printf("%d ", bag->vectors[i]->elements[j]);
        }
        printf("\n");
    }
    for(int i = 1; i < 3; i++){
        printf("Sentence %d contains %d words\n", i, bag->vectors[i]->word_count);
    }
    printf("IDF vector\n");
    for(int i = 0; i < idf_vec->size; i++){
        switch(i){
            case 1:
            case 2:
            case 4: TEST_ASSERT(idf_vec->elements[i] == (double) 2);
                    break;
            default: TEST_ASSERT(idf_vec->elements[i] == (double) 1);
                    break;
        }
        // printf("%lf ", idf_vec->elements[i]);
    }
    printf("\n");
    compute_idf_vals(idf_vec);
    for(int i = 0; i < idf_vec->size; i++){
        switch(i){
            case 1:
            case 2:
            case 4: TEST_ASSERT(idf_vec->elements[i] == (double) 0);
                    break;
        }
        printf("%lf ", idf_vec->elements[i]);
    }
    printf("\n");
    tf *tfarr;
    create_tf(&tfarr, idf_vec->size);
    TEST_ASSERT(tfarr->wordVec->size == idf_vec->size);
    printf("TF wordVec\n");
    for(int i = 0; i < tfarr->wordVec->size; i++){
        printf("%d ", tfarr->wordVec->elements[i]);   
    }
    printf("\n");
    IDFVector *important_words = compute_tf_idf(bag, tfarr, idf_vec);
    for(int i = 0; i < TFVECTORS; i++){
        for(int j = 0; j < tfarr->vectors[i]->size; j++){
            printf("%.10lf ", tfarr->vectors[i]->elements[j]);            
        }
        printf("\n");
    }
    printf("Most important words\n");
    for(int i = 0; i < important_words->size; i++){
        printf("%.10lf ", important_words->elements[i]);
    }
    printf("\nAfter sorting\n");
    Vector *wordVec = copy_vector(tfarr->wordVec);
    sort_avg_tf_idf(wordVec, important_words, 0, wordVec->size - 1);
    for(int i = 0; i < wordVec->size; i++){
      printf("%d      ", wordVec->elements[i]);
    }
    printf("\n");
    for(int i = 0; i < important_words->size; i++){
      printf("%lf ", important_words->elements[i]);
    }
    wordVec = crop_vector(wordVec, 3);
    sort_important_words_indices(wordVec, 0, wordVec->size - 1);
    
    tf *tfarr_mini;
    create_tf(&tfarr_mini, wordVec->size);
    TEST_ASSERT(tfarr_mini->wordVec->size == 3);
    size_down_tf_idf(tfarr, tfarr_mini, wordVec);
    destroy_tf(&tfarr);
    
    printf("\nAFTER SIZING DOWN\n");
    for(int i = 0; i < tfarr_mini->wordVec->size; i++){
      printf("%d\t", tfarr_mini->wordVec->elements[i]);
    }
    printf("\n");
    for(int i = 0; i < TFVECTORS; i++){
      for(int j = 0; j < tfarr_mini->vectors[i]->size; j++){
        printf("%lf ", tfarr_mini->vectors[i]->elements[j]);
      }
      printf("\n");
    }
    IDFVector *new_vec = concatenate_idf_vectors(tfarr_mini->vectors[0], tfarr_mini->vectors[1]);
    printf("\nAFTER CONCAT\n");
    for(int i = 0; i < new_vec->size; i++){
      printf("%lf ", new_vec->elements[i]);
    }
    printf("\n");
    double prediction = sigmoid(f(new_vec));
    printf("prediction: %.16lf\n", prediction);
    double ground_truth = 1;
    printf("NEW COEFFICIENTS\n");
    update_coefficients(prediction, ground_truth, new_vec);
    for(int i = 0; i < COEFF_AMOUNT; i++){
      printf("%.16lf ", coefficients[i]);
    }
    prediction = sigmoid(f(new_vec));
    printf("\n2nd prediction: %.16lf\n", prediction);
    printf("NEW COEFFICIENTS\n");
    update_coefficients(prediction, ground_truth, new_vec);
    for(int i = 0; i < COEFF_AMOUNT; i++){
      printf("%.16lf ", coefficients[i]);
    }
    printf("\n");
    destroy_tf(&tfarr_mini);
    destroy_bow(&bag);
    TEST_ASSERT(bag == NULL);
    destroy_vector(&wordVec);
    destroy_idf_vector(&idf_vec);
    destroy_idf_vector(&important_words);
}

TEST_LIST = {
    {"create", test_createbag},
    {"idf", test_idf},
    {NULL, NULL}
};