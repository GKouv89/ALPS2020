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

TEST_LIST = {
    {"create set", test_singular_clique_creation},
    {NULL, NULL}
};