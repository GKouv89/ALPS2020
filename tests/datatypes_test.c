//#include <string.h>

#include "acutest.h"
#include "../datatypes.h"

struct TestVector{
    char* character;
};

struct TestVector testvectors[] = { 
                                    {.character = "hi"}, 
                                    {.character = "my"}, 
                                    {.character = "name"}, 
                                    {.character = "is"}, 
                                    {.character = "Gina"} 
                                    };
                                    
void test_list_add(void){
    info_list* list;
    list_node* node;
    list_create(&list);
    TEST_CHECK(list != NULL);
    for(int i = 0; i < 5; i++){
        node = create_node(testvectors[i].character, i);
        list_add(list, node);
    }
    list_node* temp = list->front;
    int list_nodes = 0;
    while(temp){
        TEST_CHECK(strcmp(temp->id, testvectors[list_nodes].character) == 0);
        temp = temp->next;
        list_nodes++;
    }
    TEST_CHECK(list_nodes == 5);
    temp = list->rear;
    list_nodes = 0;
    while(temp){
        TEST_CHECK(strcmp(temp->id, testvectors[4-list_nodes].character) == 0);
        temp = temp->previous;
        list_nodes++;
    }
    TEST_CHECK(list_nodes == 5);
    destroy_list(&list);
    TEST_CHECK(list == NULL);
}

TEST_LIST = {
    {"list addition", test_list_add},
    { NULL, NULL }
};
