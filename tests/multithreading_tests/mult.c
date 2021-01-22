#include "../acutest.h"
#include "../../multithreading/scheduler.h"

struct tfidf_vector{
  double value[7];
};

struct tfidf_vector tfidf_array[] = {
  [0].value[0] = 0.04875, 
  [0].value[1] = 0, 
  [0].value[2] = 0, 
  [0].value[3] = 0, 
  [0].value[4] = 0, 
  [0].value[5] = 0, 
  [0].value[6] = 0, 
  [1].value[0] = 0.0624, 
  [1].value[1] = 0.0624, 
  [1].value[2] = 0, 
  [1].value[3] = 0, 
  [1].value[4] = 0, 
  [1].value[5] = 0, 
  [1].value[6] = 0, 
  [2].value[0] = 0, 
  [2].value[1] = 0, 
  [2].value[2] = 0.0624, 
  [2].value[3] = 0.0624, 
  [2].value[4] = 0, 
  [2].value[5] = 0, 
  [2].value[6] = 0, 
  [3].value[0] = 0, 
  [3].value[1] = 0.0624, 
  [3].value[2] = 0, 
  [3].value[3] = 0.0624, 
  [3].value[4] = 0.0624, 
  [3].value[5] = 0, 
  [3].value[6] = 0, 
  [4].value[0] = 0, 
  [4].value[1] = 0, 
  [4].value[2] = 0.0975, 
  [4].value[3] = 0, 
  [4].value[4] = 0.0975, 
  [4].value[5] = 0.1725, 
  [4].value[6] = 0.1725
};

void test_scheduler_creation(){
  tf *tfarr;
  create_tf(&tfarr, 7);
  for(int i = 0; i < TFVECTORS; i++){
    for(int j = 0; j < 7; j++){
      tfarr->vectors[i]->elements[j] = tfidf_array[i].value[j];
    }
  }
  hash_map *map = create_map();
  char *node_id = malloc(2*sizeof(char));
  for(int i = 0; i < 5; i++){
    sprintf(node_id, "%d", i);
    list_node *new_node = create_node(node_id, i);
    int hash = hash_function(map, node_id);
    add_to_bucket(map, hash, new_node);
  }
  JobScheduler *sch = initialize_scheduler(2, map, tfarr);
  qelem *newJob;
  char *file_name = malloc(38*sizeof(char));
  for(int i = 0; i < 4; i++){
    sprintf(file_name, "tests/multithreading_tests/batch%d.txt", i);
    create_queue_element(&newJob, training, file_name);
    submit_job(sch, newJob);
  }
  execute_all_jobs(sch);
  free(node_id);
  free(file_name);
  destroy_map(&map);
  destroy_tf(&tfarr);
  // destroy_scheduler(sch);
}

TEST_LIST = {
  {"scheduler_creation", test_scheduler_creation},
  {NULL, NULL}
};

