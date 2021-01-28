#include "../acutest.h"
#include "../../multithreading/scheduler.h"


struct tfidf_sm_vector {
  double value[3];
};

struct tfidf_sm_vector tfidf_sm_array1[] = {
  [0].value[0] = -0.25,
  [0].value[1] = -0.25,
  [0].value[2] = -0.25,
  [1].value[0] = -0.013,
  [1].value[1] = -0.019,
  [1].value[2] = -0.005,
  [2].value[0] = -0.2,
  [2].value[1] = -0.2,
  [2].value[2] = -0.2 
};

void test_conflict_type_1_sol_1(void){
  tf *tfarr;
  create_tf(&tfarr, 3);
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      tfarr->vectors[i]->elements[j] = tfidf_sm_array1[i].value[j];
    }
  }

  hash_map *map = create_map();
  char *node_id = malloc(40*sizeof(char));
  
  strcpy(node_id, "www.ebay.com//123");
  list_node *new_node = create_node(node_id, 0);
  int hash = hash_function(map, node_id);
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[0]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[0]->name, node_id);

  strcpy(node_id, "www.ebay.com//456");
  new_node = create_node(node_id, 1);
  hash = hash_function(map, node_id);    
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[1]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[1]->name, node_id);

  strcpy(node_id, "www.amazon.net//123");
  new_node = create_node(node_id, 2);
  hash = hash_function(map, node_id);
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[2]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[2]->name, node_id);

  double coeffs[7] = {2, 2, 2, 2, 2, 2, 2};
  conflict_resolution(map, tfarr, 0.5, "tests/multithreading_tests/conflict.txt", coeffs);
  int correct_predictions = 0;
  int all_preds = test(map, tfarr, "tests/multithreading_tests/conflict.txt", 0.5, coeffs, &correct_predictions);
  printf("ACCURACY: %.3lf\n", ((double)correct_predictions/(double)all_preds)*100);
}

struct tfidf_sm_vector tfidf_sm_array2[] = {
  [0].value[0] = -0.25,
  [0].value[1] = -0.25,
  [0].value[2] = -0.25,
  [1].value[0] = -0.013,
  [1].value[1] = -0.019,
  [1].value[2] = -0.005,
  [2].value[0] = -0.3,
  [2].value[1] = -0.3,
  [2].value[2] = -0.3 
};

void test_conflict_type_1_sol_2(void){
  tf *tfarr;
  create_tf(&tfarr, 3);
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      tfarr->vectors[i]->elements[j] = tfidf_sm_array2[i].value[j];
    }
  }

  hash_map *map = create_map();
  char *node_id = malloc(40*sizeof(char));
  
  strcpy(node_id, "www.ebay.com//123");
  list_node *new_node = create_node(node_id, 0);
  int hash = hash_function(map, node_id);
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[0]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[0]->name, node_id);

  strcpy(node_id, "www.ebay.com//456");
  new_node = create_node(node_id, 1);
  hash = hash_function(map, node_id);    
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[1]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[1]->name, node_id);

  strcpy(node_id, "www.amazon.net//123");
  new_node = create_node(node_id, 2);
  hash = hash_function(map, node_id);
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[2]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[2]->name, node_id);

  double coeffs[7] = {2, 2, 2, 2, 2, 2, 2};
  conflict_resolution(map, tfarr, 0.5, "tests/multithreading_tests/conflict.txt", coeffs);
  int correct_predictions = 0;
  int all_preds = test(map, tfarr, "tests/multithreading_tests/conflict.txt", 0.5, coeffs, &correct_predictions);
  printf("ACCURACY: %.3lf\n", ((double)correct_predictions/(double)all_preds)*100);
}

struct tfidf_sm_vector tfidf_sm_array3[] = {
  [0].value[0] = -0.35,
  [0].value[1] = -0.35,
  [0].value[2] = -0.35,
  [1].value[0] = -0.013,
  [1].value[1] = -0.019,
  [1].value[2] = -0.005,
  [2].value[0] = -0.25,
  [2].value[1] = -0.25,
  [2].value[2] = -0.25 
};

void test_conflict_type_2_sol_1(void){
  tf *tfarr;
  create_tf(&tfarr, 3);
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      tfarr->vectors[i]->elements[j] = tfidf_sm_array3[i].value[j];
    }
  }

  hash_map *map = create_map();
  char *node_id = malloc(40*sizeof(char));
  
  strcpy(node_id, "www.ebay.com//123");
  list_node *new_node = create_node(node_id, 0);
  int hash = hash_function(map, node_id);
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[0]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[0]->name, node_id);

  strcpy(node_id, "www.ebay.com//456");
  new_node = create_node(node_id, 1);
  hash = hash_function(map, node_id);    
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[1]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[1]->name, node_id);

  strcpy(node_id, "www.amazon.net//123");
  new_node = create_node(node_id, 2);
  hash = hash_function(map, node_id);
  add_to_bucket(map, hash, new_node);
  tfarr->vectors[2]->name = malloc(40*sizeof(char));
  strcpy(tfarr->vectors[2]->name, node_id);

  double coeffs[7] = {2, 2, 2, 2, 2, 2, 2};
  conflict_resolution(map, tfarr, 0.5, "tests/multithreading_tests/conflict.txt", coeffs);
  int correct_predictions = 0;
  int all_preds = test(map, tfarr, "tests/multithreading_tests/conflict.txt", 0.5, coeffs, &correct_predictions);
  printf("ACCURACY: %.3lf\n", ((double)correct_predictions/(double)all_preds)*100);
}


TEST_LIST = {
  {"conflict_type1_solution1", test_conflict_type_1_sol_1},
  {"conflict_type1_solution2", test_conflict_type_1_sol_2},
  {"conflict_type2_solution1", test_conflict_type_2_sol_1},
  {NULL, NULL}
};