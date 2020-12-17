#include "acutest.h"
#include "../BOW/bow.h"
#include "../BOW/vectorOps.h"

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
    Vector *idf_vec;
    create_vector(&idf_vec);
    
    for(int i = 0; i < 8; i++){
        if(first_sentence[i].word_no >= bag->vectors[0]->size){
            new_word_in_bag(bag, 1, idf_vec);
        }else{
            old_word(bag, 1, first_sentence[i].word_no, idf_vec);
        }
    }
    for(int i = 0; i < 5; i++){
        if(second_sentence[i].word_no >= bag->vectors[0]->size){
            new_word_in_bag(bag, 2, idf_vec);
        }else{
            old_word(bag, 2, second_sentence[i].word_no, idf_vec);
        }
    }
    printf("\n");
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < bag->vectors[i]->size; j++){
            printf("%d ", bag->vectors[i]->elements[j]);
        }
        printf("\n");
    }
    printf("IDF vector\n");
    for(int i = 0; i < idf_vec->size; i++){
        switch(i){
            case 1:
            case 2:
            case 4: TEST_ASSERT(idf_vec->elements[i] == 2);
                    break;
            default: TEST_ASSERT(idf_vec->elements[i] == 1);
                    break;
        }
        printf("%d ", idf_vec->elements[i]);
    }
    printf("\n");
    destroy_bow(&bag);
    TEST_ASSERT(bag == NULL);
    destroy_vector(&idf_vec);
}

TEST_LIST = {
    {"create", test_createbag},
    {"idf", test_idf},
    {NULL, NULL}
};