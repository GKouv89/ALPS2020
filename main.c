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
#include "negcl.h"

#include "BOW/bow.h"
#include "BOW/dictionary.h"
#include "BOW/stopwords.h"
#include "BOW/vectorOps.h"

#include "TF-IDF/idfVectorOps.h"
#include "TF-IDF/tf.h"

#include "logreg.h"

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
    fprintf(stderr, "Words parsed, dictionary done, BoW too.\n");
    
    csvparser(map, all_cliques);
    fprintf(stderr, "Positive association cliques OK\n");
    
    print_all_cliques(0, all_cliques);
    // medium 46666 with first row
    // large 341930 -//-
    
    compute_idf_vals(idf_vec);
    fprintf(stderr, "IDF completed\n");
    tf *tfarr;
    create_tf(&tfarr, idf_vec->size);
    fprintf(stderr, "TF array completed\n");
    IDFVector *important_words = compute_tf_idf(bag, tfarr, idf_vec);
    fprintf(stderr, "AVG TF-IDF values computed\n");
    destroy_bow(&bag);
    fprintf(stderr, "destroyed BoW\n");
    destroy(&l);
    fprintf(stderr, "Destroyed stopword list\n");
    destroy_tree(&dict);
    fprintf(stderr, "destroyed dictionary\n");
    
    Vector *wordVec = copy_vector(tfarr->wordVec);
    sort_avg_tf_idf(wordVec, important_words, 0, wordVec->size - 1);
    fprintf(stderr, "AVG TF-IDF values sorted\n");
    destroy_idf_vector(&important_words);
    fprintf(stderr, "Important word vector destroyed\n");

    Vector *new_wordVec = crop_vector(wordVec, IMPWORDS);
    fprintf(stderr, "wordVec cropped\n");
    destroy_vector(&wordVec);

    sort_important_words_indices(new_wordVec, 0, new_wordVec->size - 1);

    tf *tfarr_mini;
    create_tf(&tfarr_mini, IMPWORDS);
    size_down_tf_idf(tfarr, tfarr_mini, new_wordVec);
    fprintf(stderr, "TF-IDF mini array calculated\n");
    destroy_tf(&tfarr);
    fprintf(stderr, "Destroyed tf array\n");
    
    init_coefficients();
    for(int i = 0; i < COEFF_AMOUNT; i++){
      assert(coefficients[i] == 0);
    }

    train(map, tfarr_mini);
    validate(map, tfarr_mini);
    fprintf(stderr, "TEST SET\n");
    test(map, tfarr_mini);

    destroy_tf(&tfarr_mini);
    fprintf(stderr, "Destroyed mini tf array\n");

    destroy_map(&map);
    fprintf(stderr, "Destroyed map\n");
    destroy_clique_list(&all_cliques);
    fprintf(stderr, "destroyed cliques\n");
    destroy_idf_vector(&idf_vec);
    fprintf(stderr, "destroyed idf vector\n");
    destroy_vector(&new_wordVec);
    fprintf(stderr, "destroyed new_wordVec\n");
}
