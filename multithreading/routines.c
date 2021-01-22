#include <assert.h>
#include "logreg.h"
#include "parse.h"

void train(hash_map *map, tf *tfarr_new, char *file_name, double res_coeffs[]){
  FILE *fp = fopen(file_name, "r");
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
  
  while(!feof(fp)){
    bytes_read = getline(&line_buffer, &line_size, fp);
    // while(bytes_read != -1){
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
      update_coefficients(res_coeffs, prediction, (double) ground_truth, temp_vector);
      free(temp_vector->elements);
      free(temp_vector);
      bytes_read = getline(&line_buffer, &line_size, fp);
    // }
  }
  assert(fclose(fp) == 0);
  free(line_buffer);
}

double test(hash_map *map, tf *tfarr_new, char *file_name){
  FILE *fp = fopen(file_name, "r");
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
  while(!feof(fp)){
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
  assert(all_predictions == 512);
  assert(fclose(fp) == 0);
  free(line_buffer);
  return correct_predictions;
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

void update_coefficients(double res_coeffs[], double prediction, double ground_truth, IDFVector *vec){
  double cross_entropy = prediction - ground_truth;
  res_coeffs[0] +=  cross_entropy;
  for(int i = 0; i < COEFF_AMOUNT - 1; i++){
    res_coeffs[i+1] += cross_entropy*(vec->elements[i]);
  }
}