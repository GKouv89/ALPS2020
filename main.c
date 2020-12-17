#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "datatypes.h"
#include "parse.h"
#include "hashmap.h"
#include "tuplist.h"
#include "set.h"

#include "BOW/bow.h"
#include "BOW/dictionary.h"
#include "BOW/stopwords.h"
#include "BOW/vectorOps.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    
    hash_map* map = create_map();
    clique_list* all_cliques;
    create_clique_list(&all_cliques);
    
    BoW *bag;
    create_bow(&bag);
    sw_list *l;
    make_stopword_list(&l);
    tree_node *dict;
    create_tree(&dict);
    Vector *idf_vec;
    create_vector(&idf_vec);
    
    parser(map, l, bag, &dict, idf_vec);
    csvparser(map, all_cliques);
    
    // print_all_cliques(0, all_cliques);
    // print_tree(dict);
    // for(int i = 1; i < VECTORS; i++){
        // printf("VECTOR 237.\n");
        // for(int j = 0; j < bag->vectors[237]->size; j++){
            // printf("word: %d frequency: %d\n", bag->vectors[0]->elements[j], bag->vectors[237]->elements[j]);            
        // }
        // printf("\n");
    // }
    printf("IDF VECTOR\n");
    for(int i = 0; i < idf_vec->size; i++){
        printf("word %d appears in %d texts\n", i, idf_vec->elements[i]);
    }
    destroy_map(&map);
    destroy_clique_list(&all_cliques);
    destroy_bow(&bag);
    destroy(&l);
    destroy_tree(&dict);
    destroy_vector(&idf_vec);
}
