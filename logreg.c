#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "logreg.h"

void init_coefficients(){
  memset(coefficients, 0, COEFF_AMOUNT*sizeof(double));
}

void train(hash_map *map, tf *tfarr_new){
  FILE *fp = fopen(TRAINING_SET, "r");
  assert(fp != NULL);
  size_t line_size = 1024;
  size_t bytes_read;
  char *line_buffer = malloc(line_size*sizeof(char));
  char *line, *file_toked;
  char *file_name_1, *file_name_2;
  list_node *temp_1, *temp_2; 
  int ground_truth;
  int label_1 = 0;
  double prediction;
  
  int prediction_count = 0;
  
  for(int i = 0; i < EPOCHS; i++){
    fprintf(stderr, "NEW EPOCH\n");
    bytes_read = getline(&line_buffer, &line_size, fp);
    while(bytes_read != -1){
      line = line_buffer;
      file_name_1 = strtok_r(line, ",", &line);
      file_toked = strtok_r(line, ",", &line);
      while(memchr(file_toked, ' ', strlen(file_toked)) != NULL){
        file_name_2 = strtok_r(file_toked, " ", &file_toked);
      }
      temp_1 = find_node(map, file_name_1);
      temp_2 = find_node(map, file_name_2);
      assert(temp_1 != NULL);
      assert(temp_2 != NULL);
      assert(temp_1->vec_num >= 0);
      assert(temp_2->vec_num >= 0);
      assert(strcmp(tfarr_new->vectors[temp_1->vec_num]->name, file_name_1) ==  0);
      assert(strcmp(tfarr_new->vectors[temp_2->vec_num]->name, file_name_2) ==  0);
      ground_truth = atoi(line);
      if(ground_truth == 1 && i == 0){
        label_1++;
      }
      
      IDFVector *temp_vector=concatenate_idf_vectors(tfarr_new->vectors[temp_1->vec_num], tfarr_new->vectors[temp_2->vec_num]);
      assert(temp_vector->size == COEFF_AMOUNT - 1);
      prediction = sigmoid(f(temp_vector));
      update_coefficients(prediction, (double) ground_truth, temp_vector);
      free(temp_vector->elements);
      free(temp_vector);
      if(i == 0){
        prediction_count++;        
      }
      bytes_read = getline(&line_buffer, &line_size, fp);
    }
    if(i == 0){
      fprintf(stderr, "%% of ones to all: %.10lf\n", ((double)label_1/(double)(prediction_count))*100);
      fprintf(stderr, "%% of zeros to all: %.10lf\n", ((double)(prediction_count - label_1)/(double)(prediction_count))*100);      
    }
    validate(map, tfarr_new);
    fseek(fp, 0, SEEK_SET);
  }

  assert(fclose(fp) == 0);
  free(line_buffer);
}

void validate(hash_map *map, tf *tfarr_new){
  FILE *fp = fopen(VALIDATION_SET, "r");
  assert(fp != NULL);
  size_t line_size = 1024;
  size_t bytes_read;
  char *line_buffer = malloc(line_size*sizeof(char));
  char *line, *file_toked;
  char *file_name_1, *file_name_2;
  list_node *temp_1, *temp_2; 
  int ground_truth;
  int label_1 = 0;
  int correct_predictions = 0;
  int all_predictions = 0;
  int no_of_ones_guessed_correctly = 0;
  int no_of_zeros_guessed_correctly = 0;
  double prediction;
  double max_one_pred = 0.0;
  double max_zero_pred = 0.0;
  
  bytes_read = getline(&line_buffer, &line_size, fp);
  while(bytes_read != -1){
    line = line_buffer;
    file_name_1 = strtok_r(line, ",", &line);
    file_toked = strtok_r(line, ",", &line);
    while(memchr(file_toked, ' ', strlen(file_toked)) != NULL){
      file_name_2 = strtok_r(file_toked, " ", &file_toked);
    }
    temp_1 = find_node(map, file_name_1);
    temp_2 = find_node(map, file_name_2);
    assert(temp_1 != NULL);
    assert(temp_2 != NULL);
    assert(temp_1->vec_num >= 0);
    assert(temp_2->vec_num >= 0);
    assert(strcmp(tfarr_new->vectors[temp_1->vec_num]->name, file_name_1) ==  0);
    assert(strcmp(tfarr_new->vectors[temp_2->vec_num]->name, file_name_2) ==  0);
    ground_truth = atoi(line);
    
    IDFVector *temp_vector=concatenate_idf_vectors(tfarr_new->vectors[temp_1->vec_num], tfarr_new->vectors[temp_2->vec_num]);
    assert(temp_vector->size == COEFF_AMOUNT - 1);
    
    prediction = sigmoid(f(temp_vector));
    if(ground_truth == 1){
      label_1++;
      if(prediction > max_one_pred){
        max_one_pred = prediction;
      }
    }else{
      if(prediction > max_zero_pred){
        max_zero_pred = prediction;
      }
    }
    free(temp_vector->elements);
    free(temp_vector);
    
    if(prediction > 0.00041){
      prediction = 1.0;
    }else{
      prediction = 0.0;
    }
    
    if(prediction == (double) ground_truth){
      correct_predictions++;
      if(ground_truth == 1){
        no_of_ones_guessed_correctly++;
      }else{
        no_of_zeros_guessed_correctly++;
      }
    }
    all_predictions++;
    
    bytes_read = getline(&line_buffer, &line_size, fp);
  }
  fprintf(stderr, "Accuracy: %.10lf %%\n", ((double)correct_predictions/(double)all_predictions)*100);
  fprintf(stderr, "Ratio of ones to zeros: %.10lf\n", ((double)label_1/(double)(all_predictions - label_1))*100);
  fprintf(stderr, "%% of ones guessed correctly: %.10lf %%\n", ((double)no_of_ones_guessed_correctly/(double)label_1)*100);
  fprintf(stderr, "%% of zeros guessed correctly: %.10lf %%\n", ((double)no_of_zeros_guessed_correctly/(double)(all_predictions - label_1)*100));
  assert(fclose(fp) == 0);
  free(line_buffer);
}

void test(hash_map *map, tf *tfarr_new){
  FILE *fp = fopen(TEST_SET, "r");
  assert(fp != NULL);
  size_t line_size = 1024;
  size_t bytes_read;
  char *line_buffer = malloc(line_size*sizeof(char));
  char *line, *file_toked;
  char *file_name_1, *file_name_2;
  list_node *temp_1, *temp_2; 
  int ground_truth;
  int label_1 = 0;
  int correct_predictions = 0;
  int all_predictions = 0;
  int no_of_ones_guessed_correctly = 0;
  int no_of_zeros_guessed_correctly = 0;
  double prediction;
  double max_one_pred = 0.0;
  double max_zero_pred = 0.0;
  
  bytes_read = getline(&line_buffer, &line_size, fp);
  while(bytes_read != -1){
    line = line_buffer;
    file_name_1 = strtok_r(line, ",", &line);
    file_toked = strtok_r(line, ",", &line);
    while(memchr(file_toked, ' ', strlen(file_toked)) != NULL){
      file_name_2 = strtok_r(file_toked, " ", &file_toked);
    }
    temp_1 = find_node(map, file_name_1);
    temp_2 = find_node(map, file_name_2);
    assert(temp_1 != NULL);
    assert(temp_2 != NULL);
    assert(temp_1->vec_num >= 0);
    assert(temp_2->vec_num >= 0);
    assert(strcmp(tfarr_new->vectors[temp_1->vec_num]->name, file_name_1) ==  0);
    assert(strcmp(tfarr_new->vectors[temp_2->vec_num]->name, file_name_2) ==  0);
    ground_truth = atoi(line);
    
    IDFVector *temp_vector=concatenate_idf_vectors(tfarr_new->vectors[temp_1->vec_num], tfarr_new->vectors[temp_2->vec_num]);
    assert(temp_vector->size == COEFF_AMOUNT - 1);
    
    prediction = sigmoid(f(temp_vector));
    if(ground_truth == 1){
      label_1++;
      if(prediction > max_one_pred){
        max_one_pred = prediction;
      }
    }else{
      if(prediction > max_zero_pred){
        max_zero_pred = prediction;
      }
    }
    free(temp_vector->elements);
    free(temp_vector);
    
    if(prediction > 0.00041){
      prediction = 1.0;
    }else{
      prediction = 0.0;
    }
    
    if(prediction == (double) ground_truth){
      correct_predictions++;
      if(ground_truth == 1){
        no_of_ones_guessed_correctly++;
      }else{
        no_of_zeros_guessed_correctly++;
      }
    }
    all_predictions++;
    
    bytes_read = getline(&line_buffer, &line_size, fp);
  }
  fprintf(stderr, "Accuracy: %.10lf %%\n", ((double)correct_predictions/(double)all_predictions)*100);
  fprintf(stderr, "Ratio of ones to zeros: %.10lf\n", ((double)label_1/(double)(all_predictions - label_1)));
  fprintf(stderr, "Ratio of ones guessed correctly: %.10lf %%\n", ((double)no_of_ones_guessed_correctly/(double)label_1)*100);
  fprintf(stderr, "Ratio of zeros guessed correctly: %.10lf %%\n", ((double)no_of_zeros_guessed_correctly/(double)(all_predictions - label_1)*100));
  assert(fclose(fp) == 0);
  free(line_buffer);
}

double f(IDFVector *vec){
  double result = coefficients[0];
  for(int i = 0; i < COEFF_AMOUNT-1; i++){
    result += coefficients[i+1]*vec->elements[i];
  }
  return result;
}

double sigmoid(double f_x){
  double denominator = exp((-1)*f_x);
  denominator += 1;
  return 1/denominator;
}

void update_coefficients(double prediction, double ground_truth, IDFVector *vec){
  double coefficients_old[COEFF_AMOUNT];
  double coefficient_diffs[COEFF_AMOUNT];
  memset(coefficient_diffs, 0, COEFF_AMOUNT*sizeof(double));
  for(int i = 0; i < COEFF_AMOUNT; i++){
    coefficients_old[i] = coefficients[i];
  }
  double cross_entropy = prediction - ground_truth;
  cross_entropy *= 0.1; // learning_rate
  coefficients[0] = coefficients[0] - cross_entropy;
  for(int i = 0; i < COEFF_AMOUNT - 1; i++){
    coefficients[i+1] = coefficients[i+1] - cross_entropy*(vec->elements[i]);
  }
  for(int i = 0; i < COEFF_AMOUNT; i++){
    coefficient_diffs[i] = coefficients[i] - coefficients_old[i];
  }
}
