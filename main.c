#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "datatypes.h"
#include "parse.h"
#include "hashmap.h"
#include "tuplist.h"
#include "set.h"

#include "BOW/bow.h"
#include "BOW/dictionary.h"
#include "BOW/stopwords.h"
#include "BOW/vectorOps.h"

#include "TF-IDF/idfVectorOps.h"
#include "TF-IDF/tf.h"

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
    IDFVector *idf_vec;
    create_idf_vector(&idf_vec);
    
    parser(map, l, bag, &dict, idf_vec);
    csvparser(map, all_cliques);
    
    // print_all_cliques(0, all_cliques);
    print_tree(dict);
    compute_idf_vals(idf_vec);
    tf *tfarr;
    create_tf(&tfarr, idf_vec->size);
    IDFVector *important_words = compute_tf_idf(bag, tfarr, idf_vec);
    Vector *wordVec = copy_vector(tfarr->wordVec);
    sort_avg_tf_idf(wordVec, important_words, 0, wordVec->size - 1);
    printf("MOST IMPORTANT WORDS\n");
    for(int i = 0; i < wordVec->size; i++){
        printf("word no %d has avg tf-idf val of %lf\n", wordVec->elements[i], important_words->elements[i]);
    }
    destroy_map(&map);
    destroy_clique_list(&all_cliques);
    destroy_bow(&bag);
    destroy(&l);
    destroy_tree(&dict);
    destroy_idf_vector(&idf_vec);
    destroy_idf_vector(&important_words);
    destroy_vector(&wordVec);
}
