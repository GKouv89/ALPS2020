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

double test(hash_map *map, tf *tfarr_new, char *file_name, double threshold, double res_coeffs[], int *corr_preds){
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
    
    if(prediction >= threshold){
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

void threshold_tuning(hash_map *map, tf *tfarr_new, char *file_name, double coeffs[]){
  FILE *fp = fopen(file_name, "r");
  assert(fp != NULL);
  size_t line_size = 1024;
  size_t bytes_read;
  char *line_buffer = malloc(line_size*sizeof(char));
  char *line, *file_toked;
  char *file_name_1, *file_name_2;
  list_node *temp_1, *temp_2; 
  int ground_truth;
  double prediction;
  
  // int prediction_count = 0;
  int pos_pred_correct = 0; // aka whatever the model will predict as 'above' the threshold and is indeed ground_truth = 1
  int pos_pred_all = 0;  // aka anything the model will predict as 'above' the threshold
  int label_1 = 0; // actual ground_truth = 1 count
  
  double thresholds[10000];
  thresholds[0] = 0;
  for(int i = 1; i < 10000; i ++){
    thresholds[i] = thresholds[i-1] + 0.0001;
  }
  double precision = 0;
  double recall = 0;
  double best_f_measure = 0;
  double curr_f_measure = 0;
  int threshold_for_best_f_measure = 0;
  
  for(int i = 0; i < 10000; i++){
    while(!feof(fp)){
      bytes_read = getline(&line_buffer, &line_size, fp);
      if(bytes_read == -1){
        break;
      }
      line = line_buffer;
      file_name_1 = strtok_r(line, ",", &line);
      file_toked = strtok_r(line, ",", &line);  
      file_name_2 = strtok_r(file_toked, " ", &file_toked);
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
      prediction = sigmoid(f(temp_vector, coeffs));
      if(prediction >= thresholds[i]){
        if(ground_truth == 1){
          pos_pred_correct++;
        }
        pos_pred_all++;
      }
      free(temp_vector->elements);
      free(temp_vector);
    }
    precision = (double)pos_pred_correct/(double)pos_pred_all;
    recall = (double)pos_pred_correct/(double)label_1;
    curr_f_measure = (2*precision*recall)/(precision + recall);
    if(curr_f_measure > best_f_measure){
      best_f_measure = curr_f_measure;
      threshold_for_best_f_measure = i;
    }
    fseek(fp, 0, SEEK_SET);
    pos_pred_correct = 0;
    pos_pred_all = 0;
    label_1 = 0;
  }
  fprintf(stderr, "threshold = %lf, F-Measure = %lf\n", thresholds[threshold_for_best_f_measure], best_f_measure);
  assert(fclose(fp) == 0);
  free(line_buffer);
}

void conflict_resolution(hash_map* map, tf* tfarr_new, double threshold, char *file_name, double coeffs[]){
    FILE* fp;
    fp = fopen(file_name,"r");
    assert(fp != NULL);
    size_t line_size = 1024;
    size_t bytes_read;
    char *line_buffer = malloc(line_size*sizeof(char));
    char *line, *file_toked;
    char *file_name_1, *file_name_2;
    list_node *temp_1, *temp_2, *root_a, *root_b; 
    int ground_truth;
    int totalconflicts = 0;
    int conflict_resolved = 0;
    // int label_1 = 0;
    double prediction, min_prediction, max_prediction = 0;
    
    clique_list* temp_cliques;
    double res_coeffs[COEFFAMOUNT];
    for(int i = 0; i < COEFFAMOUNT; i++){
      res_coeffs[i] = 0;
    }
    for(int i = 0; i < 100; i++){
      create_clique_list(&temp_cliques);
      while(!feof(fp)){
          bytes_read = getline(&line_buffer, &line_size, fp);
          if(bytes_read == -1){
            break;
          }
          line = line_buffer;
          file_name_1 = strtok_r(line, ",", &line);
          file_toked = strtok_r(line, ",", &line);
          file_name_2 = strtok_r(file_toked, " ", &file_toked);
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
          prediction = sigmoid(f(temp_vector, coeffs));
          root_a = find_root(temp_1);
          root_b = find_root(temp_2);
             
          if(prediction >= threshold){
              // positive clique
              join_sets(temp_cliques, root_a, root_b);
              if(temp_1->matches == NULL){
                matchlist_create(&temp_1);
              }
              if(temp_2->matches == NULL){
                matchlist_create(&temp_2);
              }
              matchlist_add(temp_1, temp_2, prediction);
          }
          if(prediction > max_prediction){
            max_prediction = prediction;
          }
          free(temp_vector->elements);
          free(temp_vector);
      }
      fseek(fp, 0, SEEK_SET);
      while(!feof(fp)){
          bytes_read = getline(&line_buffer, &line_size, fp);
          if(bytes_read == -1){
            break;
          }
          line = line_buffer;
          file_name_1 = strtok_r(line, ",", &line);
          file_toked = strtok_r(line, ",", &line);
          file_name_2 = strtok_r(file_toked, " ", &file_toked);
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
          prediction = sigmoid(f(temp_vector, coeffs));
          root_a = find_root(temp_1);
          root_b = find_root(temp_2);
             
          if(prediction < threshold){
            if(root_a != root_b){ // ignoring conflicts...
              if(root_a->ngl == NULL){
                neglist_create(&root_a);
              }
              if(root_b->ngl == NULL){
                  neglist_create(&root_b);
              }
              neglist_add(root_a,root_b); 
              if(temp_1->matches == NULL){
                matchlist_create(&temp_1);
              }
              if(temp_2->matches == NULL){
                matchlist_create(&temp_2);
              }
              matchlist_add(temp_1, temp_2, prediction);
            }
          }
          free(temp_vector->elements);
          free(temp_vector);
      }
      // normalize_predictions(map, max_prediction);
      // Third pass, actual conflict resolution
      fseek(fp, 0, SEEK_SET);
      while(!feof(fp)){
          bytes_read = getline(&line_buffer, &line_size, fp);
          if(bytes_read == -1){
            break;
          }
          line = line_buffer;
          file_name_1 = strtok_r(line, ",", &line);
          file_toked = strtok_r(line, ",", &line);
          file_name_2 = strtok_r(file_toked, " ", &file_toked);
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
          prediction = sigmoid(f(temp_vector, coeffs));
          root_a = find_root(temp_1);
          root_b = find_root(temp_2);
          if(prediction < threshold){
            if(root_a == root_b){ // TYPE ONE CONFLICT
              matchlist *matches_1 = temp_1->matches;
              matchlist *matches_2 = temp_2->matches;
              if(matches_1 == NULL || matches_2 == NULL){
                fprintf(stderr, "???\n");
              }else{
                match_tuple *match_1 = matches_1->head;
                match_tuple *match_2;
                while(match_1){
                  match_2 = matches_2->head;
                  while(match_2){
                    if(strcmp(match_1->matched_with->id, match_2->matched_with->id) == 0 && match_1->prediction >= threshold && match_2->prediction >= threshold){
                      double prediction_mult = match_1->prediction*match_2->prediction;
                      if(max_prediction - prediction > prediction_mult){
                        fprintf(stderr, "case1 solution1\n");
                        fprintf(stderr, "prediction1 = %.16lf, prediction2 = %.16lf, prediction = %.16lf\n", match_1->prediction, match_2->prediction, prediction);
                        // WE BREAK THE CLIQUE,
                        // The actual data structure will not change
                        // But what will happen is we will update the weights to correspond to what we would've expected them to
                        // If we have that a and b match and a and c match and we find out it is likelier for
                        // b and c not to match, then we must update the coefficients by 'separating'
                        // them in two cliques. Which of the two matching is more likely to be true?
                        // a matches b or a matches c? That one stays. The other one has its coeffs updated.
                        if(match_1->prediction > match_2->prediction){
                          list_node *to_change_1 = temp_1;
                          list_node *to_change_2 = find_node(map, match_1->matched_with->id);
                          IDFVector *to_change = concatenate_idf_vectors(tfarr_new->vectors[to_change_1->vec_num], tfarr_new->vectors[to_change_2->vec_num]);
                          double to_change_pred = sigmoid(f(to_change, coeffs));
                          update_coefficients(res_coeffs, to_change_pred, 0, to_change);
                          free(to_change->elements);
                          free(to_change);
                        }else{
                          list_node *to_change_1 = find_node(map, match_1->matched_with->id);
                          list_node *to_change_2 = temp_2;
                          IDFVector *to_change = concatenate_idf_vectors(tfarr_new->vectors[to_change_1->vec_num], tfarr_new->vectors[to_change_2->vec_num]);
                          double to_change_pred = sigmoid(f(to_change, coeffs));
                          update_coefficients(res_coeffs, to_change_pred, 0, to_change);
                          free(to_change->elements);
                          free(to_change);
                        }
                      }else{
                        fprintf(stderr, "prediction1 = %.16lf, prediction2 = %.16lf, prediction = %.16lf\n", match_1->prediction, match_2->prediction, prediction);
                        update_coefficients(res_coeffs, prediction, 1, temp_vector);
                      }
                      totalconflicts++;
                      conflict_resolved = 1;
                      break;
                    }
                    match_2 = match_2->next;
                  }
                  if(conflict_resolved == 1){
                    conflict_resolved = 0;
                    break;
                  }
                  match_1 = match_1->next;
                }
              }
            }
          }else{
            // we search to see if root_a has root_b in its neglist; that would be a conflict
            neg_list *root_a_neg_list = root_a->ngl;
            if(root_a->ngl != NULL){
              neg_node *root_a_temp = root_a_neg_list->front;
              while(root_a_temp){
                if(root_a_temp->neg_clique == root_b){ // TYPE 2 CONFLICT
                  IDFVector *rep_relationship = concatenate_idf_vectors(tfarr_new->vectors[root_a->vec_num], tfarr_new->vectors[root_b->vec_num]);
                  double prediction_repres = sigmoid(f(rep_relationship, coeffs));
                  if(prediction_repres >= threshold){
                    fprintf(stderr, "the representatives also don't match...?");
                  }else{
                    if(max_prediction - prediction_repres > prediction){
                      // It's more likely that temp_1 and temp_2's representatives don't match
                      // So it's more likely they also don't
                      update_coefficients(res_coeffs, prediction, 0, temp_vector);
                    }else{
                      // It's more likely that temp_1 and temp_2 match so the representatives also must match
                      update_coefficients(res_coeffs, prediction_repres, 1, temp_vector);
                    }
                    totalconflicts++;
                  }
                  free(rep_relationship->elements);
                  free(rep_relationship);
                  break;
                }
                root_a_temp = root_a_temp->next_in_negclique;
              }
            }
          }
          free(temp_vector->elements);
          free(temp_vector);
      }
      fprintf(stderr, "Total conflicts found in this run\t%d\n", totalconflicts);
      fseek(fp, 0, SEEK_SET);
      destroy_clique_list(&temp_cliques);
      reinitialize_all_cliques(map);
      if(totalconflicts == 0){
        break;
      }
      for(int j = 0; j < COEFFAMOUNT; j++){
        res_coeffs[j] = res_coeffs[j]/totalconflicts;
        res_coeffs[j] *= 0.1;
        coeffs[j] = coeffs[j] - res_coeffs[j];
        res_coeffs[j] = 0.0;
      }
      totalconflicts = 0;
    }
}