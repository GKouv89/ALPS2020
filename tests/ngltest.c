#include "acutest.h"
#include "../set.h"
#include "../datatypes.h"
#include "../hashmap.h"
#include "../parse.h"
#include "../negcl.h"

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
    [9].matching = 1,
    // CLIQUE 2
    [10].left_spec = "www.henrys.com//22",
    [10].right_spec = "www.shopbot//1234",
    [10].matching = 1,
    // Case 2: a,b=1, a,c=0 -> b,c=0
    // EXPECTED OUTPUT: ΌΛΑ ΤΗΣ 1ΗΣ ΚΛΙΚΑΣ ΝΑ ΜΗΝ ΤΑΙΡΙΑΖΟΥΝ
    // ΜΕ ΟΛΑ ΤΗΣ 2ΗΣ ΚΛΙΚΑΣ
    // [11].left_spec = "buy.net//5641",
    // [11].right_spec = "www.henrys.com//22",
    // [11].matching = 0

    // [10].left_spec = "www.ebay.com//360240",
    // [10].right_spec = "buy.net//5641",
    // [10].matching = 1,
    // [11].left_spec = "www.ebay.com//360240",
    // [11].right_spec = "www.shopbot//34567",
    // [11].matching = 0,
    //
    // Case 3: a,b=0 , a,c=0 -> b,c =?
    // EXPECTED OUTPUT: ΔΕΝ ΘΕΛΟΥΜΕ ΕΚΤΥΠΩΣΗ ΑΡΝΗΤΙΚΗΣ ΣΥΣΧΕΤΙΣΗΣ
    // ΜΕΤΑΞΥ ΤΩΝ ΜΕΛΩΝ ΤΩΝ 2 ΚΛΙΚΩΝ
    // [11].left_spec = "www.flipkart//5943",
    // [11].right_spec = "buy.net//5641",
    // [11].matching = 0,
    // [12].left_spec = "www.flipkart//5943",
    // // [12].right_spec = "www.ebay.com//59612",
    // [12].right_spec = "www.henrys.com//22",
    // [12].matching = 0,
    //
    //Case 4: a,b=0, a,c=1 -> b,c=0
    [11].left_spec = "www.henrys.com//22",
    [11].right_spec = "buy.net//5641",
    [11].matching = 0
};


void test_negclique_creation(void){
    hash_map* map = create_map();
    list_node* node;
    int hash;
    for(int i = 0; i < 12; i++){
        node = create_node(lines[i].right_spec);
        hash = hash_function(map,lines[i].right_spec);
        add_to_bucket(map, hash, node);
        node = create_node(lines[i].left_spec);
        hash = hash_function(map,lines[i].left_spec);
        add_to_bucket(map, hash, node);
    }
    print_bucket_no_of_entries(map);
    list_node *a, *b;
    list_node *root;
    clique_list *l;
    create_clique_list(&l);
    for(int i = 0; i < 12; i++){
        if(lines[i].matching){
            a = find_node(map, lines[i].left_spec);
            TEST_CHECK(a != NULL);
            b = find_node(map, lines[i].right_spec);
            TEST_CHECK(b != NULL);
            root = join_sets(l, a, b);
            TEST_CHECK(root != NULL);
        }
    }
    printf("PRINTING POSITIVE CLIQUES\n");
    print_all_cliques(1, l);
    printf("END OF POSITIVE CLIQUES\n\n\n");
    for(int i = 0; i < 12; i++){
        if(lines[i].matching==0){
            list_node *a, *b;
            a = find_root(find_node(map, lines[i].left_spec));
            TEST_CHECK(a != NULL);
            b = find_root(find_node(map, lines[i].right_spec));
            TEST_CHECK(b != NULL);
            if(a->ngl == NULL){
                neglist_create(&a);
            }
            if(b->ngl == NULL){
                neglist_create(&b);
            }
            neglist_add(a,b); 
        }
    }
    clique_list_node *temp = l->front;
    printf("PRINTING NEGATIVE CLIQUES\n");
    while(temp){
        neglist_print(temp->representative);
        temp = temp->next;
    }
}

// void test_big_negclique_creation(void){
    // srand(time(NULL));
    
    // hash_map* map = create_map();
    // clique_list* all_cliques;
    // create_clique_list(&all_cliques);
    // csvparser(map, all_cliques);
    
    // print_all_cliques(0, all_cliques);
    // destroy_map(&map);
    // destroy_clique_list(&all_cliques);

// }

TEST_LIST = {
    {"create_negclique", test_negclique_creation},
    // {"create_all_negs", test_big_negclique_creation},
    {NULL, NULL}
};
