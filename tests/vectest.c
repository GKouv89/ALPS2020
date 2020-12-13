#include "acutest.h"
#include "../BOW/vectorOps.h"

void test_veccreate(void){
    Vector *vec;
    create_vector(&vec);
    TEST_ASSERT(vec != NULL);
    TEST_ASSERT(vec->elements != NULL);
}

void test_vecinsert(void){
    Vector *vec;
    create_vector(&vec);
    for(int i = 0; i < 100; i++){
        insert_element(vec);
    }
    for(int i = 0; i < 100; i++){
        TEST_ASSERT(vec->elements[i] == 1);
    }
}

void test_vecinsertoverflow(void){
    Vector *vec;
    create_vector(&vec);
    for(int i = 0; i < 200; i++){
        insert_element(vec);
    }
    for(int i = 0; i < 200; i++){
        TEST_ASSERT(vec->elements[i] == 1);
    }
}

TEST_LIST = {
    {"create", test_veccreate},
    {"insert", test_vecinsert},
    {"insert_overflow", test_vecinsertoverflow},
    {NULL, NULL}
};