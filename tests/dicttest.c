#include "acutest.h"
#include "../BOW/dictionary.h"

struct word{
    char word[50];
};

struct word sentence[] = {
    [0].word = "the",
    [1].word = "dog",
    [2].word = "jumped",
    [3].word = "at",
    [4].word = "the",
    [5].word = "fox"
};

void test_treeInsertion(void){
    tree_node *tree;
    create_tree(&tree);
    TEST_ASSERT(tree == NULL);
    int error = 0;
    for(int i = 0; i < 6; i++){
        insert_tree(&tree, sentence[i].word, i, &error);
        if(i == 4){
            TEST_ASSERT(error == 1);
        }else{
            TEST_ASSERT(error == 0);            
        }
    }
    for(int i = 0; i < 6; i++){
        insert_tree(&tree, sentence[i].word, i, &error);
        TEST_ASSERT(error == 1);
    }
    destroy_tree(&tree);
    TEST_ASSERT(tree == NULL);
}

void test_treeSearch(void){
    tree_node *tree;
    create_tree(&tree);
    int error = 0;
    for(int i = 0; i < 6; i++){
        insert_tree(&tree, sentence[i].word, i, &error);
    }
    for(int i = 0; i < 6; i++){
        insert_tree(&tree, sentence[i].word, i, &error);
    }
    for(int i = 0; i < 6; i++){
        if(i == 4){
            TEST_ASSERT(search_tree(tree, sentence[i].word) == 0);            
        }else{
            TEST_ASSERT(search_tree(tree, sentence[i].word) == i);
        }
    }
    destroy_tree(&tree);
}

TEST_LIST = {
    {"insert", test_treeInsertion},
    {"search", test_treeSearch},
    {NULL, NULL}
};