#include <string.h>
#include <assert.h>
#include <math.h>
#include "scheduler.h"
#include "routines.h"

#include "../tuplist.h"
#include "../negcl.h"
#include "../set.h"

int train(hash_map *map, tf *tfarr_new, char *file_name, double curr_coeffs[], double res_coeffs[]){
  // fprintf(stderr, "train: %s\n", file_name);
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
    if(bytes_read == -1){
      // fprintf(stderr, "BATCH PIECE %s FINISHED\n", file_name);
      break;
    }
    // fprintf(stderr, "BATCH: %s line: %s | prediction no %d\n", file_name, line_buffer, prediction_count);
    line = line_buffer;
    file_name_1 = strtok_r(line, ",", &line);
    file_toked = strtok_r(line, ",", &line);  
    // while(memchr(file_toked, ' ', strlen(file_toked)) != NULL){
      file_name_2 = strtok_r(file_toked, " ", &file_toked);
    // }
    // fprintf(stderr, "BATCH: %s line %d | file_name_1, file_name_2: %s, %s\n",  file_name, prediction_count, file_name_1, file_name_2);
    temp_1 = find_node(map, file_name_1);
    temp_2 = find_node(map, file_name_2);
    assert(temp_1 != NULL);
    assert(temp_2 != NULL);
    assert(temp_1->vec_num >= 0);
    assert(temp_2->vec_num >= 0);
    assert(strcmp(tfarr_new->vectors[temp_1->vec_num]->name, file_name_1) ==  0);
    assert(strcmp(tfarr_new->vectors[temp_2->vec_num]->name, file_name_2) ==  0);
    ground_truth = atoi(line);
    if(ground_truth == 1){
      label_1++;
    }
    
    IDFVector *temp_vector=concatenate_idf_vectors(tfarr_new->vectors[temp_1->vec_num], tfarr_new->vectors[temp_2->vec_num]);
    assert(temp_vector->size == COEFFAMOUNT - 1);
    prediction = sigmoid(f(temp_vector, curr_coeffs));
    update_coefficients(res_coeffs, prediction, (double) ground_truth, temp_vector);
    free(temp_vector->elements);
    free(temp_vector);
    prediction_count++;
  }
  assert(fclose(fp) == 0);
  free(line_buffer);
  return prediction_count;
}

double test(hash_map *map, tf *tfarr_new, char *file_name, double res_coeffs[], int *corr_preds){
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
    assert(temp_vector->size == COEFFAMOUNT - 1);
    
    prediction = sigmoid(f(temp_vector, res_coeffs));
    free(temp_vector->elements);
    free(temp_vector);
    
    if(prediction > 0.0315){
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
  assert(fclose(fp) == 0);
  free(line_buffer);
  *corr_preds = correct_predictions;
  return all_predictions;
}

double f(IDFVector *vec, double res_coeffs[]){
  double result = res_coeffs[0];
  for(int i = 0; i < COEFFAMOUNT-1; i++){
    result += res_coeffs[i+1]*vec->elements[i];
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
  for(int i = 0; i < COEFFAMOUNT - 1; i++){
    res_coeffs[i+1] += cross_entropy*(vec->elements[i]);
  }
}

void conflict_resolution(hash_map* map, tf* tfarr_new, double coeffs[]){
    FILE* fp;
    int threshold = 0.0315;
    fp = fopen("ML_Sets/ValidationSet_medium.csv","r");
    assert(fp != NULL);
    size_t line_size = 1024;
    size_t bytes_read;
    char *line_buffer = malloc(line_size*sizeof(char));
    char *line, *file_toked;
    char *file_name_1, *file_name_2;
    list_node *temp_1, *temp_2, *root_a, *root_b; 
    int ground_truth;
    // int label_1 = 0;
    double prediction, min_prediction, max_prediction;
    min_prediction = 1;
    max_prediction = 0;
    
    clique_list* temp_cliques;
    create_clique_list(&temp_cliques);
    while(!feof(fp)){
        bytes_read = getline(&line_buffer, &line_size, fp);
        if(bytes_read == -1){
          break;
        }
        line = line_buffer;
        file_name_1 = strtok_r(line, ",", &line);
        file_toked = strtok_r(line, ",", &line);
        // while(memchr(file_toked, ' ', strlen(file_toked)) != NULL){
            file_name_2 = strtok_r(file_toked, " ", &file_toked);
        // }
        temp_1 = find_node(map, file_name_1);
        temp_2 = find_node(map, file_name_2);
        assert(temp_1 != NULL);
        assert(temp_2 != NULL);
        assert(temp_1->vec_num >= 0);
        assert(temp_2->vec_num >= 0);
        assert(strcmp(tfarr_new->vectors[temp_1->vec_num]->name, file_name_1) ==  0);
        assert(strcmp(tfarr_new->vectors[temp_2->vec_num]->name, file_name_2) ==  0);
        ground_truth = atoi(line);
        // if(ground_truth == 1 && i == 0){
            // label_1++;
        // }
        IDFVector *temp_vector=concatenate_idf_vectors(tfarr_new->vectors[temp_1->vec_num], tfarr_new->vectors[temp_2->vec_num]);
        prediction = sigmoid(f(temp_vector, coeffs));
        root_a = find_root(temp_1);
        root_b = find_root(temp_2);
         
        if(prediction > threshold){
            // positive clique
            join_sets(temp_cliques, root_a, root_b);
        }else{
            // negative clique
            if(root_a->ngl == NULL){
                neglist_create(&root_a);
            }
            if(root_b->ngl == NULL){
                neglist_create(&root_b);
            }
            neglist_add(root_a, root_b);
        }
        if(prediction < min_prediction){
          min_prediction = prediction;
        }
        if(prediction > max_prediction){
          max_prediction = prediction;
        }
        printf("PREDICTION: %s, %s, %.16lf\n", file_name_1, file_name_2, prediction);
        free(temp_vector->elements);
        free(temp_vector);
    }
    clique_list_node* tempnode;
    tempnode = temp_cliques->front;
    list_node* temp_listnode;
    neg_node* negt;
    int frinc = 0; // found representative in own neglist
    int totalconflicts = 0;
    while(tempnode != NULL){
        temp_listnode = tempnode->representative;
        if(temp_listnode->ngl != NULL){
          negt = temp_listnode->ngl->front;
          while(negt != NULL){
              if(strcmp(temp_listnode->id,negt->neg_clique->id) == 0){
                  frinc = 1;
                  break;
              }
              negt = negt->next_in_negclique;
          }
          if(frinc!=0){
              totalconflicts++;
          }
          frinc = 0;
        }
        tempnode = tempnode->next;          
    }
    fprintf(stderr, "MIN PRED: %.16lf, MAX PRED: %.16lf\n", min_prediction, max_prediction);
    fprintf(stderr, "Total conflicts found\t%d\n",totalconflicts);
    printf("VALIDATION CLIQUES\n");
    // print_all_cliques(0, temp_cliques);
    destroy_clique_list(&temp_cliques);
    reinitialize_all_cliques(map);
}