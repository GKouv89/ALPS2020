#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "scheduler.h"
#include <pthread.h>
#include "../TF-IDF/tf.h"
#include "../hashmap.h"

JobScheduler* initialize_scheduler(int execution_threads, hash_map* map, tf* tfar){
  JobScheduler *new = malloc(sizeof(JobScheduler));
  new->execution_threads = execution_threads;
  new->threads_complete = 0;
  new->time_to_work = malloc(execution_threads*sizeof(int)); // We assume that all can take a job in the beginning 
  for(int i = 0; i < execution_threads; i++){
    new->time_to_work[i] = 1;
  }
  new->read_from_coeff_array = 1; //Set to 1 in order to calculate coefficients first and then accuracy
  
  //Storing Hashmap and Tf array in scheduler
  new->map = map;
  new->tf_array = tfar;
  ///////////////////////////////////////////
  // create job queue
  int qerror;
  new->q = NULL;
  queue_create(&(new->q),&qerror);
  if(qerror){
      printf("Failed to create job queue!\n");
  }
  /////////////////////
  //Thread array
  new->tids = malloc(sizeof(pthread_t)*execution_threads);
  ///////////////
  // Initialization of mutexes and cond variables
  new->are_calcs_done_cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
  new->can_i_take_a_job = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
  new->queue_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
  new->threads_complete_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
  new->can_i_take_a_job_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
  new->print_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; // Mainly for debugging purposes
  /////////////////////////////////////////////////////////////////////////////////////
  // Initialization of common 'memory'/arrays where threads will write their results //
  // And from where the master thread will read to calculate new common coefficients //
  ///////////////////////////////// Or overall accuracy ///////////////////////////////
  for(int i = 0; i < COEFFAMOUNT; i++){
    new->coefficients[i] = 0;
  }
  new->threads_coeffs = malloc(execution_threads*sizeof(*(new->threads_coeffs)));
  for(int i = 0; i < execution_threads; i++){
    for(int j = 0; j < COEFFAMOUNT; j++){
      new->threads_coeffs[i][j] = 0;
    }
  }
  new->thread_predictions = calloc(execution_threads, sizeof(int));
  new->thread_correct_predictions = calloc(execution_threads, sizeof(int));
  new->all_predictions_testing = 0;
  new->all_correct_predictions = 0;
}

int submit_job(JobScheduler* sch, qelem* j){
  // this function is responsible for adding jobs to the queue
  // will be called multiple times from main
  queue_insert(sch->q,j);
}

void* threadWork(void *arg){
  JobScheduler *myScheduler = arg;
  qelem *aJob;
  int error;
  int i;
  while(1){
    pthread_mutex_lock(&myScheduler->print_mutex);
    printf("in threadWork\n");
    pthread_mutex_unlock(&myScheduler->print_mutex);
    for(i = 0; i < myScheduler->execution_threads; i++){
      if(myScheduler->tids[i] == pthread_self()){
        break;
      }
    }
    // printf("i = %d\n", i);
    assert(i < myScheduler->execution_threads);

    pthread_mutex_lock(&myScheduler->can_i_take_a_job_mutex);
    while(myScheduler->time_to_work[i] == 0){
      pthread_mutex_lock(&myScheduler->print_mutex);
      printf("queue cond variable false\n");
      pthread_mutex_unlock(&myScheduler->print_mutex);
      pthread_cond_wait(&myScheduler->can_i_take_a_job, &myScheduler->can_i_take_a_job_mutex); 
    }
    // printf("queue cond variable true\n");
    pthread_mutex_lock(&myScheduler->queue_mutex);
    // printf("in queue critical_area\n"); 
    aJob = queue_remove(myScheduler->q, &error);
    // printf("job popped\n");
    pthread_mutex_unlock(&myScheduler->queue_mutex); 
    pthread_cond_signal(&myScheduler->can_i_take_a_job);
    pthread_mutex_unlock(&myScheduler->can_i_take_a_job_mutex);
    if(error == 1){
      return NULL;
    }
    
    if(aJob->type == training){
      for(int j = 0; j < COEFFAMOUNT; j++){
        myScheduler->threads_coeffs[i][j] = 0;      
      }
      myScheduler->thread_predictions[i] = train(myScheduler->map, myScheduler->tf_array, aJob->file_name, myScheduler->coefficients, myScheduler->threads_coeffs[i]);
    }else if(aJob->type == testing){
      pthread_mutex_lock(&myScheduler->threads_complete_mutex);
      // pthread_mutex_lock(&myScheduler->threads_complete_mutex);
      if(myScheduler->read_from_coeff_array == 1){
        myScheduler->read_from_coeff_array = 0;
      }
      pthread_mutex_unlock(&myScheduler->threads_complete_mutex);
      myScheduler->thread_predictions[i] = test(myScheduler->map, myScheduler->tf_array, aJob->file_name, myScheduler->coefficients, &myScheduler->thread_correct_predictions[i]);
    }
    
    pthread_mutex_lock(&myScheduler->threads_complete_mutex);
    // printf("thread complete\n");
    myScheduler->threads_complete++;
    myScheduler->time_to_work[i] = 0;
    destroy_queue_element(&aJob);
    pthread_cond_signal(&myScheduler->are_calcs_done_cond);  
    pthread_mutex_unlock(&myScheduler->threads_complete_mutex);
  }
  return NULL;
}


int execute_all_jobs(JobScheduler* sch){
  for(int i = 0; i < sch->execution_threads; i++){
    pthread_create(&sch->tids[i], NULL, threadWork, (void*)sch );
  }
  int to_break = 0;  
  while(1){
    pthread_mutex_lock(&sch->threads_complete_mutex);
    while(sch->threads_complete != sch->execution_threads){
      pthread_mutex_lock(&sch->print_mutex);
      printf("calcs are done cond var is false\n");
      pthread_mutex_unlock(&sch->print_mutex);
      pthread_cond_wait(&sch->are_calcs_done_cond, &sch->threads_complete_mutex); 
    }
    // printf("calcs are done cond var is true\n");
    if(sch->read_from_coeff_array == 1){
      int batch_size = 0;
      for(int i = 0; i < sch->execution_threads; i++){
        batch_size += sch->thread_predictions[i];
      }
      double coefficients_temp[COEFFAMOUNT];
      for(int i = 0; i < COEFFAMOUNT; i++){
        coefficients_temp[i] = 0;
      }
      // calculate new coefficients (still in training phase)
      for(int i = 0; i < COEFFAMOUNT; i++){
        for(int j = 0; j < sch->execution_threads; j++){
          coefficients_temp[i] += sch->threads_coeffs[j][i];
        }
        coefficients_temp[i] = coefficients_temp[i]/((double)batch_size);
        coefficients_temp[i] *= 0.1; // learning rate
      }
      for(int i = 0; i < COEFFAMOUNT; i++){
        sch->coefficients[i] = sch->coefficients[i] - coefficients_temp[i];
      }
    }else{
      for(int i = 0; i < sch->execution_threads; i++){
        sch->all_correct_predictions += sch->thread_correct_predictions[i];
        sch->all_predictions_testing += sch->thread_predictions[i];
      }
      pthread_mutex_lock(&sch->print_mutex);
      printf("sch->all_correct_predictions = %d\n", sch->all_correct_predictions);
      printf("sch->all_predictions_testing = %d\n", sch->all_predictions_testing);
      pthread_mutex_unlock(&sch->print_mutex);      
    }
    sch->threads_complete = 0;
    pthread_mutex_lock(&sch->can_i_take_a_job_mutex);
    for(int i = 0; i < sch->execution_threads; i++){
      pthread_mutex_lock(&sch->print_mutex);
      printf("updating sch->time_to_work\n");
      pthread_mutex_unlock(&sch->print_mutex);
      sch->time_to_work[i] = 1;
    }
    // printf("new loop of batches about to begin\n");
    pthread_cond_signal(&sch->can_i_take_a_job);
    pthread_mutex_unlock(&sch->can_i_take_a_job_mutex);
    // pthread_cond_signal(&sch->are_calcs_done_cond);
    pthread_mutex_unlock(&sch->threads_complete_mutex); 
    pthread_mutex_lock(&sch->queue_mutex);
    if(queue_empty(sch->q)){
      to_break = 1;
    }
    pthread_mutex_unlock(&sch->queue_mutex);
    if(to_break){
      printf("Accuracy: %lf%%\n", ((double)sch->all_correct_predictions/(double)sch->all_predictions_testing)*100);
      break;
    }
    pthread_mutex_lock(&sch->print_mutex);
    printf("next main loop...\n");
    pthread_mutex_unlock(&sch->print_mutex);
  }
  // the order of the two pthread_cond_signal may have to change
}

int destroy_scheduler(JobScheduler** sch){
  // destroy cond vars and semaphores - is it necessary before thread destruction?
  pthread_mutex_destroy(&((*sch)->queue_mutex));
  pthread_mutex_destroy(&((*sch)->threads_complete_mutex));
  pthread_mutex_destroy(&((*sch)->can_i_take_a_job_mutex));
  pthread_mutex_destroy(&((*sch)->print_mutex));
  pthread_cond_destroy(&((*sch)->are_calcs_done_cond));
  pthread_cond_destroy(&((*sch)->can_i_take_a_job));
  free((*sch)->time_to_work);
  (*sch)->time_to_work = NULL;
  free((*sch)->tids);
  (*sch)->tids = NULL;
  free((*sch)->thread_predictions);
  (*sch)->thread_predictions = NULL;
  free((*sch)->thread_correct_predictions);
  (*sch)->thread_correct_predictions = NULL;
  free((*sch)->threads_coeffs);
  (*sch)->map = NULL;
  (*sch)->tf_array = NULL;
  // destroy all threads
  // destroy queue
  queue_destroy(&((*sch)->q));
  // destroy pthread tids
  // destroy scheduler itself
  free(*sch);
  *sch = NULL;
}

int decrement(char *path, int execution_threads, int first_no){
    FILE* fp = fopen(path, "r");
    assert(fp != NULL);
    FILE* tempfile;
    int i;
    int ncount;
    char* filename;
    char* line;
    size_t sizel = 300;
    int batchsize = 512/execution_threads; // actual batch size divided by how many threads the batch will
    // be broken down into
    line = malloc(sizeof(char)*sizel);
    filename = malloc(sizeof("batch1000.csv"));   
    ncount = 0;
    while(1){
        i = 0;
        sprintf(filename,"batch%d.csv",first_no);
        tempfile = fopen(filename, "w+");
        ncount++;
        first_no++;
        while(i<batchsize){
            getline(&line, &sizel, fp);
            fputs(line,tempfile);
            i++;
        }
        if(feof(fp)){
            break;
        }
        assert(fclose(tempfile) == 0);
    }
    int checking = ncount%execution_threads;
    FILE* tomerge;
    FILE* temp;
    int newbatch = 0;
    if(checking != 0){
        i = ncount;
        tomerge = fopen("nonfullbatch.csv","w+");
        while(i != ncount - checking){
            sprintf(filename,"batch%d.csv",i);
            temp = fopen(filename, "r");
            while(!feof(temp)){
                getline(&line, &sizel, temp);
                fputs(line, tomerge);
                newbatch++;
            }
            fclose(temp);
            remove(filename);
            i--;
        }
        // int temploop = ncount - checking;
        ncount -= checking ;
        first_no -= checking;
        fseek(tomerge,0, SEEK_SET);
        tomerge = fopen("nonfullbatch.csv","r");
        int linesperbatch = newbatch/execution_threads;
        for(i = first_no+1; i <= first_no+execution_threads; i++){
            sprintf(filename,"batch%d.csv",i);
            temp = fopen(filename, "w+");
            for(int j=0; j<linesperbatch; j++){
                getline(&line, &sizel, tomerge);
                fputs(line, temp);
            }
            fclose(temp);
        }
        ncount += execution_threads;
        fclose(tomerge);
    }
    
    assert(fclose(fp) == 0);
    return ncount;
}