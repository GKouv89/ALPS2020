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
  new->time_to_work = 1; // We assume that all can take a job in the beginning 
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
  new->thread_correct_predictions = calloc(execution_threads, sizeof(int));;
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
  
  pthread_mutex_lock(&myScheduler->queue_mutex);
  printf("in queue critical_area\n");  
  while(myScheduler->time_to_work == 0){
    printf("queue cond variable false\n");
    pthread_cond_wait(&myScheduler->can_i_take_a_job, &myScheduler->queue_mutex); 
  }
  printf("queue cond variable true\n");
  if(!queue_empty(myScheduler->q)){
    printf("more jobs\n");
    aJob = queue_remove(myScheduler->q, &error);
    assert(error == 0);
  }else{
    printf("no more jobs\n");
    return NULL;
  }
  printf("job popped\n");
  pthread_mutex_unlock(&myScheduler->queue_mutex); 
  pthread_cond_signal(&myScheduler->can_i_take_a_job);

  int i;
  for(i = 0; i < myScheduler->execution_threads; i++){
    if(myScheduler->tids[i] == pthread_self()){
      break;
    }
  }
  printf("i = %d\n", i);
  assert(i < myScheduler->execution_threads);
  
  if(aJob->type == training){
    for(int j = 0; j < COEFFAMOUNT; j++){
      myScheduler->threads_coeffs[i][j] = 0;      
    }
    myScheduler->thread_predictions[i] = train(myScheduler->map, myScheduler->tf_array, aJob->file_name, myScheduler->coefficients, myScheduler->threads_coeffs[i]);
  }// }else if(aJob->type == testing){
    // myScheduler->read_from_coeff_array == 1;
    // myScheduler->thread_correct_predictions[i] = test(myScheduler->map, myScheduler->tf_array, aJob->file_name, myScheduler->coefficients);
  // }
  
  pthread_mutex_lock(&myScheduler->threads_complete_mutex);
  printf("thread complete\n");
  myScheduler->threads_complete++;
  // if(aJob->type == testing){
    // myScheduler->test_batches++;
  // }
  destroy_queue_element(&aJob);
  pthread_mutex_unlock(&myScheduler->threads_complete_mutex);
  pthread_cond_signal(&myScheduler->are_calcs_done_cond);  
}


int execute_all_jobs(JobScheduler* sch){
  for(int i = 0; i < sch->execution_threads; i++){
    pthread_create(&sch->tids[i], NULL, threadWork, (void*)sch );
  }

  while(!queue_empty(sch->q)){
    pthread_mutex_lock(&sch->threads_complete_mutex);
    while(sch->threads_complete != sch->execution_threads){
      printf("calcs are done cond var is false\n");
      pthread_cond_wait(&sch->are_calcs_done_cond, &sch->threads_complete_mutex); 
    }
    printf("calcs are done cond var is true\n");
    sch->time_to_work = 0;
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
      printf("new coeffs done\n");
    }
    // else{
      // for(int i = 0; i < sch->execution_threads; i++){
        // sch->all_correct_predictions += sch->thread_correct_predictions[i];
      // }
    // }
    sch->threads_complete = 0;
    pthread_mutex_unlock(&sch->threads_complete_mutex); 
    // pthread_cond_signal(&sch->are_calcs_done_cond);
    sch->time_to_work = 1;
    printf("new loop of batches about to begin\n");
    pthread_cond_signal(&sch->can_i_take_a_job);
  }
  // the order of the two pthread_cond_signal may have to change
}

int destroy_scheduler(JobScheduler* sch){
  // destroy cond vars and semaphores - is it necessary before thread destruction?
  // destroy all threads
  // destroy queue
  queue_destroy(&(sch->q));
  // destroy pthread tids
  // destroy scheduler itself
}

void decrement(){
    FILE* fp = fopen("../ML_Sets/TrainingSet_medium.csv", "r");
    assert(fp != NULL);
    FILE* tempfile;
    int i;
    int ncount;
    char* filename;
    char* line;
    size_t sizel = 300;
    int batchsize = 512;
    line = malloc(sizeof(char)*sizel);
    filename = malloc(sizeof("batch1000.csv"));
    ncount = 1;
    while(1){
        i = 0;
        sprintf(filename,"batch%d.csv",ncount);
        tempfile = fopen(filename, "w+");
        ncount++;
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
    assert(fclose(fp) == 0);
}