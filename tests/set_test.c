#include "acutest.h"
#include "../set.h"
#include "../datatypes.h"
#include "../hashmap.h"

struct csv_line{
    char* left_spec;
    char* right_spec;
    int matching;
};

struct csv_line lines[] = {
    [0].left_spec = "buy.net//5641",
    [0].right_spec = "ebay.com//54040",
    [0].matching = 0,
    [1].left_spec = "buy.net//5641",
    [1].right_spec = "www.pricedekho.com//1022",
    [1].matching = 0,
    [2].left_spec = "buy.net//5641",
    [2].right_spec = "www.shopbot.com.au//388",
    [2].matching = 0,
    [3].left_spec = "buy.net//5641",
    [3].right_spec = "www.shopmania.in//974",
    [3].matching = 0,
    [4].left_spec = "buy.net//5641",
    [4].right_spec = "www.mypriceindia.com//22",
    [4].matching = 0,
    [5].left_spec = "buy.net//5641",
    [5].right_spec = "www.price-hunt.com//9794",
    [5].matching = 1,
    [6].left_spec = "buy.net//5641",
    [6].right_spec = "www.ebay.com//54217",
    [6].matching = 1,
    [7].left_spec = "buy.net//5641",
    [7].right_spec = "www.gosale.com//849",
    [7].matching = 1,
    [8].left_spec = "buy.net//5641",
    [8].right_spec = "www.ebay.com//58588",
    [8].matching = 1,    
    [9].left_spec = "buy.net//5641",
    [9].right_spec = "www.ebay.com//59612",
    [9].matching = 1  
};

struct csv_line output[] = {
    [0].left_spec = "buy.net//5641,",
    [0].right_spec = "www.price-hunt.com//9794",
    [1].left_spec = "buy.net//5641,",
    [1].right_spec = "www.ebay.com//54217",
    [2].left_spec = "buy.net//5641,",
    [2].right_spec = "www.gosale.com//849",
    [3].left_spec = "buy.net//5641,",
    [3].right_spec = "www.ebay.com//58588",
    [4].left_spec = "buy.net//5641,",
    [4].right_spec = "www.ebay.com//59612",
    
    [5].left_spec = "www.price-hunt.com//9794,",
    [5].right_spec = "www.ebay.com//54217",
    [6].left_spec = "www.price-hunt.com//9794,",
    [6].right_spec = "www.gosale.com//849",
    [7].left_spec = "www.price-hunt.com//9794,",
    [7].right_spec = "www.ebay.com//58588",
    [8].left_spec = "www.price-hunt.com//9794,",
    [8].right_spec = "www.ebay.com//59612",
    
    [9].left_spec = "www.ebay.com//54217,",
    [9].right_spec = "www.gosale.com//849",
    [10].left_spec = "www.ebay.com//54217,",
    [10].right_spec = "www.ebay.com//58588",
    [11].left_spec = "www.ebay.com//54217,",
    [11].right_spec = "www.ebay.com//59612",
    
    [12].left_spec = "www.gosale.com//849,",
    [12].right_spec = "www.ebay.com//58588",
    [13].left_spec = "www.gosale.com//849,",
    [13].right_spec = "www.ebay.com//59612",
    
    [14].left_spec = "www.ebay.com//58588,",
    [14].right_spec = "www.ebay.com//59612"
};

struct csv_line mult_input[] = {
    [0].left_spec = "www.garricks.com.au//31",
    [0].right_spec = "www.ebay.com//53278",
    
    [1].left_spec = "www.aliexpress.com//456", //2nd clique
    [1].right_spec = "www.amazon.com//987",
    
    [2].left_spec = "www.ebay.com//43998",
    [2].right_spec = "www.ebay.com//53278",
    
    [3].left_spec = "www.ebay.com//60484",
    [3].right_spec = "www.ebay.com//53278",
    
    [4].left_spec = "www.gosale.com//953",  //2nd clique
    [4].right_spec = "www.ebay.com//42902",
    
    [5].left_spec = "www.ebay.com//60484",
    [5].right_spec = "www.ebay.com//56576",
    
    [6].left_spec = "buy.net//345", //2nd clique
    [6].right_spec = "www.ebay.com//42902",
    
    [7].left_spec = "www.ebay.com//56576",
    [7].right_spec = "www.ebay.com//47121",  
    
    [8].left_spec = "www.gosale.com//953", //2nd clique
    [8].right_spec = "www.aliexpress.com//456"
};

void test_singular_clique_creation(void){
    hash_map* map = create_map();
    list_node* node;
    int hash;
    for(int i = 0; i < 10; i++){
        node = create_node(lines[i].right_spec);
        hash = hash_function(map,lines[i].right_spec);
        add_to_bucket(map, hash, node);
    }
    node = create_node(lines[0].left_spec);
    hash = hash_function(map,lines[0].left_spec);
    add_to_bucket(map, hash, node);
    print_bucket_no_of_entries(map);
    list_node *a, *b;
    list_node *root;
    for(int i = 0; i < 10; i++){
        if(lines[i].matching){
            a = find_node(map, lines[i].left_spec);
            TEST_CHECK(a != NULL);
            b = find_node(map, lines[i].right_spec);
            TEST_CHECK(b != NULL);
            root = join_sets(a, b);
            TEST_CHECK(root != NULL);
        }
    }
    print_clique(1, root);
    FILE* fp = fopen("clique.txt", "r");
    TEST_CHECK(fp != NULL);
    char buf[255], buf2[255];
    for(int i = 0; i < 15; i++){
        fscanf(fp, "%s", buf);
        fscanf(fp, "%s", buf2);
        TEST_CHECK((strcmp(buf, output[i].left_spec) == 0) && (strcmp(buf2, output[i].right_spec) == 0));
    }
    fclose(fp);
}

void test_clique_insert(void){
    clique_list* list;
    create_clique_list(&list);
    TEST_CHECK(list->front == NULL);
    TEST_CHECK(list->rear == NULL);
    list_node* node;
    for(int i = 0; i < 9; i++){
        node = create_node(mult_input[i].left_spec);
        insert_master(list, node);
        node = create_node(mult_input[i].right_spec);
        insert_master(list, node);
    }
    clique_list_node* temp = list->front;
    int i = 0;
    while(temp){
        TEST_CHECK(strcmp(temp->representative->id, mult_input[i].left_spec) == 0);
        temp = temp->next;
        TEST_CHECK(strcmp(temp->representative->id, mult_input[i].right_spec) == 0);
        TEST_MSG("rep = %s, i = %d, right_spec = %s", temp->representative->id, i, mult_input[i].right_spec);
        temp = temp->next;
        i++;
    }
}

void test_clique_remove(void){
    clique_list* list;
    create_clique_list(&list);
    TEST_CHECK(list->front == NULL);
    TEST_CHECK(list->rear == NULL);
    list_node *node0, *node1, *node2, *node3;
    node0 = create_node(mult_input[0].left_spec);
    insert_master(list, node0);
    node1 = create_node(mult_input[0].right_spec);
    insert_master(list, node1);
    node2 = create_node(mult_input[1].left_spec);
    insert_master(list, node2);
    node3 = create_node(mult_input[1].right_spec);
    insert_master(list, node3);
    
    remove_master(list, node1); // test a simple removal, 2nd element
    TEST_CHECK(list->front != NULL);
    TEST_CHECK(list->rear != NULL);
    remove_master(list, node0); // test removal of element in front position
    TEST_CHECK(list->front != NULL);
    TEST_CHECK(list->rear != NULL);
    remove_master(list, node3); // test removal of element in rear position
    TEST_CHECK(list->front != NULL);
    TEST_CHECK(list->rear != NULL);
    remove_master(list, node2); // test removal of only element
    TEST_CHECK(list->front == NULL);
    TEST_CHECK(list->rear == NULL);
}

void test_multiple_clique_creation(void){
    hash_map* map = create_map();
    list_node* node;
    int hash;
    for(int i = 0; i < 9; i++){
        node = create_node(mult_input[i].right_spec);
        hash = hash_function(map,mult_input[i].right_spec);
        add_to_bucket(map, hash, node);
    }
    list_node *a, *b;
    // list_node *root;
    for(int i = 0; i < 9; i++){
        a = find_node(map, mult_input[i].left_spec);
        TEST_CHECK(a != NULL);
        b = find_node(map, mult_input[i].right_spec);
        TEST_CHECK(b != NULL);
        root = join_sets(a, b);
        TEST_CHECK(root != NULL);
    }
} 

TEST_LIST = {
    {"create_one_clique", test_singular_clique_creation},
    {"create_clique_list", test_clique_insert},
    {"test_removal", test_clique_remove},
    // {"create_two_cliques", test_multiple_clique_creation},
    {NULL, NULL}
};