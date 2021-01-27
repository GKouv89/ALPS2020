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

#include "multithreading/scheduler.h"
#include "multithreading/routines.h"
#include "multithreading/queue.h"

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
    
    time_t parse_start, parse_end;
    time(&parse_start);
    parser(map, l, bag, &dict, idf_vec);
    fprintf(stderr, "Words parsed, dictionary done, BoW too.\n");
    time(&parse_end);
    fprintf(stderr, "%ld for parsing and BOW creation\n", parse_end - parse_start);
    
    time_t cliques_start, cliques_end;
    time(&cliques_start);
    csvparser(map, all_cliques);
    time(&cliques_end);
    fprintf(stderr, "%ld for clique creation\n", cliques_end - cliques_start);
    fprintf(stderr, "Positive association cliques OK\n");
    
    // print_all_cliques(0, all_cliques);
    // medium 46666 with first row
    // large 341930 -//-
    reinitialize_all_cliques(map);
    
    time_t idf_start, idf_end;
    time(&idf_start);
    compute_idf_vals(idf_vec);
    fprintf(stderr, "IDF completed\n");
    time(&idf_end);
    fprintf(stderr, "%ld for IDF computation\n", idf_end - idf_start);
    tf *tfarr;
    create_tf(&tfarr, idf_vec->size);
    fprintf(stderr, "TF array completed\n");
    time_t tf_start, tf_end;
    time(&tf_start);
    IDFVector *important_words = compute_tf_idf(bag, tfarr, idf_vec);
    time(&tf_end);
    fprintf(stderr, "%ld for tf-idf overall and average computation\n", tf_end-tf_start);
    fprintf(stderr, "AVG TF-IDF values computed\n");
    destroy_bow(&bag);
    fprintf(stderr, "destroyed BoW\n");
    destroy(&l);
    fprintf(stderr, "Destroyed stopword list\n");
    destroy_tree(&dict);
    fprintf(stderr, "destroyed dictionary\n");
    
    time_t sort_start, sort_end;
    time(&sort_start);
    Vector *wordVec = copy_vector(tfarr->wordVec);
    sort_avg_tf_idf(wordVec, important_words, 0, wordVec->size - 1);
    fprintf(stderr, "AVG TF-IDF values sorted\n");
    destroy_idf_vector(&important_words);
    fprintf(stderr, "Important word vector destroyed\n");

    Vector *new_wordVec = crop_vector(wordVec, IMPWORDS);
    fprintf(stderr, "wordVec cropped\n");
    destroy_vector(&wordVec);

    sort_important_words_indices(new_wordVec, 0, new_wordVec->size - 1);
    time(&sort_end);
    fprintf(stderr, "%ld for important word selection\n", sort_end - sort_start);
    tf *tfarr_mini;
    create_tf(&tfarr_mini, IMPWORDS);
    time_t size_down_start, size_down_end;
    time(&size_down_start);
    size_down_tf_idf(tfarr, tfarr_mini, new_wordVec);
    fprintf(stderr, "TF-IDF mini array calculated\n");
    time(&size_down_end);
    fprintf(stderr, "%ld for sizing down TF-IDF array\n", size_down_end - size_down_start);
    destroy_tf(&tfarr);
    fprintf(stderr, "Destroyed tf array\n");

    char *path;
    if(strstr(DATASET, "medium") != NULL){
      path = malloc((strlen("ML_Sets/TrainingSet_medium.csv") + 1)*sizeof(char));
      strcpy(path, "ML_Sets/TrainingSet_medium.csv");
    }else{
      path = malloc((strlen("ML_Sets/TrainingSet.csv") + 1)*sizeof(char));
      strcpy(path, "ML_Sets/TrainingSet.csv");
    }
    
    time_t time_create_start, time_create_end;
    time(&time_create_start);
    int training_files = decrement(path, THREADNO, 1);
    assert(training_files % THREADNO == 0);
    if(strstr(DATASET, "medium") != NULL){
      strcpy(path, "ML_Sets/TestSet_medium.csv");
    }else{
      stpcpy(path, "ML_Sets/TestSet.csv");
    }
    int lower_bound = training_files + 1;
    int test_files = decrement(path, THREADNO, lower_bound);
    time(&time_create_end);
    fprintf(stderr, "%ld for batch files creation\n", time_create_end - time_create_start);
    assert(test_files % THREADNO == 0);
    fprintf(stderr, "no of testing files made: %d\n", test_files);
    fprintf(stderr, "No of training files made: %d\n", training_files);
    
    ////INSERTION INTO JOB QUEUE////
    JobScheduler *sch = initialize_scheduler(THREADNO, map, tfarr_mini);
    qelem *newJob;
    char *file_name = malloc(45*sizeof(char));
    for(int i = 0; i < 5; i++){ // TRAINING EPOCHS
      for(int j = 1; j <= training_files; j++){
        sprintf(file_name, "batch%d.csv", j);
        create_queue_element(&newJob, training, file_name);      
        submit_job(sch, newJob);
      }
    }
    for(int i = lower_bound; i < lower_bound + test_files; i++){
      sprintf(file_name, "batch%d.csv", i);
      create_queue_element(&newJob, testing, file_name);      
      submit_job(sch, newJob);
    }
    assert(queue_size(sch->q) % THREADNO == 0);
    time_t machine_learning_start, machine_learning_end;
    time(&machine_learning_start);
    execute_all_jobs(sch);
    time(&machine_learning_end);
    fprintf(stderr, "%ld for 5 epochs of training and one run on test file\n", machine_learning_end - machine_learning_start);
    for(int i = 1; i < lower_bound + test_files; i++){
      sprintf(file_name, "batch%d.csv", i);
      remove(file_name);
    }

    // if(strstr(DATASET, "medium") != NULL){
      // threshold_tuning(map, tfarr_mini, "ML_Sets/TestSet_medium.csv", sch->coefficients);
    // }
    conflict_resolution(map, tfarr_mini, 0.0775, "ML_Sets/ValidationSet_medium.csv", sch->coefficients);

    double accuracy = ((double)sch->all_correct_predictions/(double)sch->all_predictions_testing)*100;
    fprintf(stderr, "ACCURACY: %lf%%\n", accuracy);    
    destroy_scheduler(&sch);
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
