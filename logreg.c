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
  int stop;
  double prediction;
  
  while(1){
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
      
      prediction = sigmoid(f(tfarr_new->vectors[temp_1->vec_num],tfarr_new->vectors[temp_2->vec_num]));
      stop = update_coefficients(prediction, (double) ground_truth, tfarr_new->vectors[temp_1->vec_num], tfarr_new->vectors[temp_2->vec_num]);
      if(stop){
        break;
      }
      bytes_read = getline(&line_buffer, &line_size, fp);
    }
    if(stop){
      break;
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
  double prediction;
  
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
    
    prediction = sigmoid(f(tfarr_new->vectors[temp_1->vec_num],tfarr_new->vectors[temp_2->vec_num]));
    if(prediction >= 0.5){
      prediction = 1.0;
    }else{
      prediction = 0.0;
    }
    if(prediction == (double) ground_truth){
      correct_predictions++;
    }
    all_predictions++;
    
    bytes_read = getline(&line_buffer, &line_size, fp);
  }
  fprintf(stderr, "Accuracy: %.10lf\n", ((double)correct_predictions/(double)all_predictions)*100);
  assert(fclose(fp) == 0);
  free(line_buffer);
}

double f(IDFVector *vec1, IDFVector *vec2){
  double result = coefficients[0];
  for(int i = 0; i < IMPWORDS; i++){
    result += coefficients[i+1]*vec1->elements[i];
  }
  for(int i = 0; i < IMPWORDS; i++){
    result += coefficients[IMPWORDS+i+1]*vec2->elements[i];
  }
  return result;
}

double sigmoid(double f_x){
  double denominator = exp((-1)*f_x);
  denominator += 1;
  return 1/denominator;
}

int update_coefficients(double prediction, double ground_truth, IDFVector *vec1, IDFVector *vec2){
  double coefficients_old[COEFF_AMOUNT];
  double coefficient_diffs[COEFF_AMOUNT];
  memset(coefficient_diffs, 0, COEFF_AMOUNT*sizeof(double));
  memcpy(coefficients_old, coefficients, COEFF_AMOUNT*sizeof(double));
  double cross_entropy = prediction - ground_truth;
  cross_entropy *= 0.1; // learning_rate
  coefficients[0] = coefficients[0] - cross_entropy;
  for(int i = 0; i < IMPWORDS; i++){
    coefficients[i+1] = coefficients[i+1] - cross_entropy*vec1->elements[i];
  }
  for(int i = 0; i < IMPWORDS; i++){
    coefficients[IMPWORDS+i+1] = coefficients[IMPWORDS+i+1] - cross_entropy*vec2->elements[i];
  }
  for(int i = 0; i < COEFF_AMOUNT; i++){
    coefficient_diffs[i] = coefficients[i] - coefficients_old[i];
  }
  double norm = 0.0;
  for(int i = 0; i < COEFF_AMOUNT; i++){
    norm += coefficient_diffs[i]*coefficient_diffs[i];
  }
  if(norm < EPSILON){
    return 1;
  }
  return 0;
}
