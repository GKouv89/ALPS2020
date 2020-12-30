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

void test_bow_insertion_and_idf_creation(void){
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
    for(int i = 0; i < bag->vectors[1]->size; i++){
      if(i == 8){
        TEST_ASSERT(bag->vectors[1]->elements[i] == 0);
      }else{
        TEST_ASSERT(bag->vectors[1]->elements[i] == 1);
      }
    }
    for(int i = 0; i < bag->vectors[2]->size; i++){
      switch(i){
        case 0:
        case 3:
        case 5:
        case 6:
        case 7: TEST_ASSERT(bag->vectors[2]->elements[i] == 0);
          break;
        case 2:
        case 8: TEST_ASSERT(bag->vectors[2]->elements[i] == 1);
                break;
        case 4: TEST_ASSERT(bag->vectors[2]->elements[i] == 2);
                break;
      }
    }
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
    TEST_ASSERT(bag->vectors[1]->word_count == 8);
    TEST_ASSERT(bag->vectors[2]->word_count == 4);
    for(int i = 1; i < 3; i++){
        printf("Sentence %d contains %d words\n", i, bag->vectors[i]->word_count);
    }
    for(int i = 0; i < idf_vec->size; i++){
        switch(i){
            case 1:
            case 2:
            case 4: TEST_ASSERT(idf_vec->elements[i] == (double) 2);
                    break;
            default: TEST_ASSERT(idf_vec->elements[i] == (double) 1);
                    break;
        }
    }
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
}

void test_tf_creation(){
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
    compute_idf_vals(idf_vec);
    
    // NEW STUFF, SPECIFIC TO THIS TEST //
    tf *tfarr;
    create_tf(&tfarr, idf_vec->size);
    TEST_ASSERT(tfarr->wordVec->size == idf_vec->size);
    for(int i = 0; i < tfarr->wordVec->size; i++){
      TEST_ASSERT(tfarr->wordVec->elements[i] == i);
    }
    IDFVector *important_words = compute_tf_idf(bag, tfarr, idf_vec);
    printf("\nTF-IDF values\n");
    for(int i = 0; i < TFVECTORS; i++){
      for(int j = 0; j < tfarr->vectors[i]->size; j++){
          printf("%.10lf ", tfarr->vectors[i]->elements[j]);            
      }
      printf("\n");
    }
}

void test_tf_clipping(){
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
    compute_idf_vals(idf_vec);
    
    tf *tfarr;
    create_tf(&tfarr, idf_vec->size);
    IDFVector *important_words = compute_tf_idf(bag, tfarr, idf_vec);
    // NEW STUFF, SPECIFIC TO THIS TEST //
    for(int i = 0; i < important_words->size; i++){
      TEST_ASSERT(important_words->elements[i] == (tfarr->vectors[0]->elements[i] + tfarr->vectors[1]->elements[i])/2);
    }
    Vector *wordVec = copy_vector(tfarr->wordVec);
    sort_avg_tf_idf(wordVec, important_words, 0, wordVec->size - 1);
    TEST_ASSERT(wordVec->size == tfarr->wordVec->size);
    TEST_ASSERT(wordVec->capacity == tfarr->wordVec->capacity);
    TEST_ASSERT(wordVec->size == important_words->size);
    for(int i = 0; i < wordVec->size; i++){
      for(int j = i+1; j < wordVec->size; j++){
        TEST_ASSERT(important_words->elements[i] >= important_words->elements[j]);
      }
    }
    wordVec = crop_vector(wordVec, 3);
    sort_important_words_indices(wordVec, 0, wordVec->size - 1);
    for(int i = 0; i < wordVec->size; i++){
      for(int j = i+1; j < wordVec->size; j++){
        TEST_ASSERT(wordVec->elements[i] < wordVec->elements[j]);
      }
    }
    tf *tfarr_mini;
    create_tf(&tfarr_mini, wordVec->size);
    TEST_ASSERT(tfarr_mini->wordVec->size == 3);
    size_down_tf_idf(tfarr, tfarr_mini, wordVec);
    
    for(int i = 0; i < wordVec->size; i++){
      TEST_ASSERT(tfarr_mini->vectors[0]->elements[i] == tfarr->vectors[0]->elements[wordVec->elements[i]]);
      TEST_ASSERT(tfarr_mini->vectors[1]->elements[i] == tfarr->vectors[1]->elements[wordVec->elements[i]]);
    }
    destroy_tf(&tfarr);
    TEST_ASSERT(tfarr == NULL);
}
    
void test_log_reg_sigmoid_and_update_coeffs(){
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
  compute_idf_vals(idf_vec);
    
  tf *tfarr;
  create_tf(&tfarr, idf_vec->size);
  IDFVector *important_words = compute_tf_idf(bag, tfarr, idf_vec);
  Vector *wordVec = copy_vector(tfarr->wordVec);
  sort_avg_tf_idf(wordVec, important_words, 0, wordVec->size - 1);
  for(int i = 0; i < wordVec->size; i++){
    for(int j = i+1; j < wordVec->size; j++){
      TEST_ASSERT(important_words->elements[i] >= important_words->elements[j]);
    }
  }
  wordVec = crop_vector(wordVec, 3);
  sort_important_words_indices(wordVec, 0, wordVec->size - 1);
  tf *tfarr_mini;
  create_tf(&tfarr_mini, wordVec->size);
  size_down_tf_idf(tfarr, tfarr_mini, wordVec);

  destroy_tf(&tfarr);
  // NEW STUFF, SPECIFIC TO THIS TEST //
  IDFVector *new_vec = concatenate_idf_vectors(tfarr_mini->vectors[0], tfarr_mini->vectors[1]);
  TEST_ASSERT(new_vec->size == tfarr_mini->vectors[0]->size + tfarr_mini->vectors[1]->size);
  for(int i = 0; i < tfarr_mini->vectors[0]->size; i++){
    TEST_ASSERT(new_vec->elements[i] == tfarr_mini->vectors[0]->elements[i]);
  }
  for(int i = 0; i < tfarr_mini->vectors[1]->size; i++){
    TEST_ASSERT(new_vec->elements[tfarr_mini->vectors[0]->size + i] == tfarr_mini->vectors[1]->elements[i]);
  }
  double prediction = sigmoid(f(new_vec));
  TEST_ASSERT(prediction == 0.5);
  double ground_truth = 1;
  printf("\nNEW COEFFICIENTS\n");
  update_coefficients(prediction, ground_truth, new_vec);
  TEST_ASSERT(coefficients[0] == 0.05);
  for(int i = 0; i < COEFF_AMOUNT; i++){
    printf("%.16lf ", coefficients[i]);
  }
  double new_prediction = sigmoid(f(new_vec));
  TEST_ASSERT(new_prediction > prediction);
  printf("\n2nd prediction: %.16lf\n", new_prediction);
  printf("NEW COEFFICIENTS\n");
  update_coefficients(new_prediction, ground_truth, new_vec);
  for(int i = 0; i < COEFF_AMOUNT; i++){
    printf("%.16lf ", coefficients[i]);
  }
  printf("\n");
  destroy_tf(&tfarr_mini);
  TEST_ASSERT(tfarr_mini == NULL);
  destroy_bow(&bag);
  TEST_ASSERT(bag == NULL);
  destroy_vector(&wordVec);
  TEST_ASSERT(wordVec == NULL);
  destroy_idf_vector(&idf_vec);
  TEST_ASSERT(idf_vec == NULL);
  destroy_idf_vector(&important_words);
  TEST_ASSERT(important_words == NULL);
}

TEST_LIST = {
    {"BoW_create", test_createbag},
    {"BoW_IDF_insert_create", test_bow_insertion_and_idf_creation},
    {"TF_insert_create", test_tf_creation},
    {"TF_clip", test_tf_clipping},
    {"Logistic_Regression_functions", test_log_reg_sigmoid_and_update_coeffs},
    {NULL, NULL}
};