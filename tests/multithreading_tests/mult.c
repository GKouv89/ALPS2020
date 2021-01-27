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
  FILE* mergfile;
  mergfile = fopen("tests/multithreading_tests/mergbatch.txt","r");
  TEST_ASSERT(mergfile!= NULL);
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
    tfarr->vectors[i]->name = malloc(2*sizeof(char));
    strcpy(tfarr->vectors[i]->name, node_id);
  }
  JobScheduler *sch = initialize_scheduler(2, map, tfarr);
  qelem *newJob;
  char *file_name = malloc(38*sizeof(char));
  for(int i = 0; i < 6; i++){
    sprintf(file_name, "tests/multithreading_tests/batch%d.txt", i);
    if(i != 4 && i != 5){
      create_queue_element(&newJob, training, file_name);
    }else{
      create_queue_element(&newJob, testing, file_name);
    }
    submit_job(sch, newJob);
  }
  
  execute_all_jobs(sch);
  
  double curr_coeffs[COEFFAMOUNT];
  for(int i = 0; i<COEFFAMOUNT; i++){
      curr_coeffs[i] = 0;
  }
  double res_coeffs[COEFFAMOUNT];
  for(int i = 0; i<COEFFAMOUNT; i++){
      res_coeffs[i] = 0;
  }
  
  //////////////////////////////////////
  // Linear execution to test correctness of results
  size_t line_size = 1024;
  size_t bytes_read;
  char *line_buffer = malloc(line_size*sizeof(char));
  char *line, *file_toked;
  char *file_name_1, *file_name_2;
  list_node *temp_1, *temp_2; 
  int ground_truth;
  int label_1 = 0;
  double prediction;
  int all_predictions = 0;
  double cross_entropy;
  while(!feof(mergfile)){
    for(int i = 0; i<4; i++){ // batchsize is 4 but because we have 2 threads, each batch is broken into 2 files
        bytes_read = getline(&line_buffer, &line_size, mergfile);
        line = line_buffer;
        file_name_1 = strtok_r(line, ",", &line);
        file_toked = strtok_r(line, ",", &line);
        file_name_2 = strtok_r(file_toked, " ", &file_toked);
        temp_1 = find_node(map, file_name_1);
        temp_2 = find_node(map, file_name_2);
        TEST_ASSERT(temp_1 != NULL);
        TEST_ASSERT(temp_2 != NULL);
        TEST_ASSERT(temp_1->vec_num >= 0);
        TEST_ASSERT(temp_2->vec_num >= 0);
        TEST_ASSERT(strcmp(tfarr->vectors[temp_1->vec_num]->name, file_name_1) ==  0);
        TEST_ASSERT(strcmp(tfarr->vectors[temp_2->vec_num]->name, file_name_2) ==  0);
        ground_truth = atoi(line);
        if(ground_truth == 1){
          label_1++;
        }
        IDFVector *temp_vector=concatenate_idf_vectors(tfarr->vectors[temp_1->vec_num], tfarr->vectors[temp_2->vec_num]);
        TEST_CHECK(temp_vector->size == COEFFAMOUNT - 1);
        prediction = sigmoid(f(temp_vector, curr_coeffs));
        // update_coefficients(res_coeffs, prediction, (double) ground_truth, temp_vector);
        cross_entropy = prediction - ground_truth;
        res_coeffs[0] +=  cross_entropy;
        for(int i = 0; i < COEFFAMOUNT - 1; i++){
          res_coeffs[i+1] += cross_entropy*(temp_vector->elements[i]);
        }
        free(temp_vector->elements);
        free(temp_vector); 
    }
    for(int i = 0; i < COEFFAMOUNT; i++){
        res_coeffs[i] = res_coeffs[i]/((double)4);
        res_coeffs[i] *= 0.1; // learning rate
    }
    for(int i = 0; i < COEFFAMOUNT; i++){
        curr_coeffs[i] = curr_coeffs[i] - res_coeffs[i];
    }
    for(int i = 0; i<COEFFAMOUNT; i++){
      res_coeffs[i] = 0;
    }
  }
  for(int i = 0; i < COEFFAMOUNT; i++){
    TEST_ASSERT(curr_coeffs[i] == sch->coefficients[i]); 
  }
  FILE* fp;
  fp = fopen("tests/multithreading_tests/mergbatchtest.txt","r");
  TEST_ASSERT(fp != NULL);
  char *file_name_3, *file_name_4;
  list_node *temp_3, *temp_4; 
  int correct_predictions = 0;
  
  while(!feof(fp)){
    bytes_read = getline(&line_buffer, &line_size, fp);
    line = line_buffer;
    file_name_3 = strtok_r(line, ",", &line);
    file_toked = strtok_r(line, ",", &line);
    // while(memchr(file_toked, ' ', strlen(file_toked)) != NULL){
      file_name_4 = strtok_r(file_toked, " ", &file_toked);
    // }
    temp_3 = find_node(map, file_name_3);
    temp_4 = find_node(map, file_name_4);
    TEST_ASSERT(temp_3 != NULL);
    TEST_ASSERT(temp_4 != NULL);
    TEST_ASSERT(temp_3->vec_num >= 0);
    TEST_ASSERT(temp_4->vec_num >= 0);
    TEST_ASSERT(strcmp(tfarr->vectors[temp_3->vec_num]->name, file_name_3) ==  0);
    TEST_ASSERT(strcmp(tfarr->vectors[temp_4->vec_num]->name, file_name_4) ==  0);
    ground_truth = atoi(line);
    
    IDFVector *temp_vector=concatenate_idf_vectors(tfarr->vectors[temp_3->vec_num], tfarr->vectors[temp_4->vec_num]);
    TEST_ASSERT(temp_vector->size == COEFFAMOUNT - 1);
    
    prediction = sigmoid(f(temp_vector, sch->coefficients));
    free(temp_vector->elements);
    free(temp_vector);
    
    if(prediction > 0.5){
      prediction = 1.0;
    }else{
      prediction = 0.0;
    }
    
    if(prediction == (double) ground_truth){
      correct_predictions++;
    }
    all_predictions++;
  }
  printf("Correct %d\tAll %d\n", correct_predictions, all_predictions);
  printf("Accuracy %lf%%\n",((double)correct_predictions/(double)all_predictions)*100);
  printf("Accuracy computed through threading: %lf%%\n", ((double)sch->all_correct_predictions/(double)sch->all_predictions_testing)*100);
  TEST_ASSERT(fclose(fp) == 0);
  
  free(line_buffer);
  fclose(mergfile);
  free(node_id);
  free(file_name);
  destroy_map(&map);
  destroy_tf(&tfarr);
  destroy_scheduler(&sch);
}

void test_batch_file_creation(void){
  char *path = malloc((strlen("ML_Sets/TrainingSet.csv") + 1)*sizeof(char));//_medium.csv") + 1)*sizeof(char));
  strcpy(path, "ML_Sets/TrainingSet.csv");
  int training_files = decrement(path, 4, 1);
  // TEST_ASSERT(training_files == 220);
  strcpy(path, "ML_Sets/TestSet_medium.csv");
  int lower_bound = training_files + 1;
  int test_files = decrement(path, 4, lower_bound);
  printf("no of testing files made: %d\n", test_files);
  // TEST_ASSERT(test_files == 76);
  ////INSERTION INTO JOB QUEUE////
  JobScheduler *sch = initialize_scheduler(4, NULL, NULL);
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
  TEST_CHECK(queue_size(sch->q) == 5*training_files + test_files);
  TEST_MSG("queue_size: %d", queue_size(sch->q));
  for(int i = 1; i < lower_bound + test_files; i++){
    sprintf(file_name, "batch%d.csv", i);
    remove(file_name);
  }
}


TEST_LIST = {
  {"scheduler_creation", test_scheduler_creation},
  {"batch_file_creation", test_batch_file_creation},
  {NULL, NULL}
};

