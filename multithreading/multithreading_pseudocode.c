#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "queueelement.h"
#include "queue.h"
#include "scheduler.h"
#include <pthreads.h>
#include "../TF-IDF/tf.h"
#include "../hashmap.h"

JobScheduler* initialize_scheduler(int execution_threads, hash_map* map, tf* tfar){
  JobScheduler new = malloc(sizeof(JobScheduler));
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
  queue_create(&(new->q),&qerror);
  if(qerror){
      printf("Failed to create job queue!\n");
  }
  /////////////////////
  //Thread array
  pthread_t threads[execution_threads];
  new->tids = threads;
  // new->tids = malloc(sizeof(pthread_t)*execution_threads);
  ///////////////
  // Initialization of mutexes and cond variables
  new->are_calcs_done_cond = PTHREAD_COND_INITIALIZER;
  new->can_i_take_a_job = PTHREAD_COND_INITIALIZER;
  new->queue_mutex = PTHREAD_MUTEX_INITIALIZER;
  new->threads_complete_mutex = PTHREAD_MUTEX_INITIALIZER;
  /////////////////////////////////////////////////
}

int submit_job(JobScheduler* sch, qelem* j){
  // this function is responsible for adding jobs to the queue
  // will be called multiple times from main
  queue_insert(sch->q,j);
}

(void *) threadWork(void *arg){
  JobScheduler *myScheduler = arg;
  qelem *aJob;
  int error;
  
  pthread_mutex_lock(&queue_mutex); 
  while(time_to_work == 0){
    pthread_cond_wait(&can_i_take_a_job, &queue_mutex); 
  }
  if(queue_empty(arg->q)){    
    aJob = queue_remove(q, &error);
    assert(error == 0);
  }else{
    return NULL;
  }
  pthread_mutex_unlock(&queue_mutex); 
  pthread_cond_signal(&can_i_take_a_job, &queue_mutex);

  int i;
  for(i = 0; i < myScheduler->execution_threads; i++){
    if(myScheduler->tids[i] == pthread_self()){
      break;
    }
  }
  assert(i < myScheduler->execution_threads);
  
  if(aJob->typeofjob == train){
    memset(threads_coeffs[i], 0, COEFF_AMOUNT*sizeof(double));
    train(myScheduler->map, myScheduler->tfarr_new, aJob->file_name, threads_coeffs[i]);
  }else if(aJob->typeofjob == test){
    read_from_coeff_array == 1;
    thread_accuracies[i] = test(myScheduler->map, myScheduler->tfarr_new, aJob->file_name);
  }
  
  pthread_mutex_lock(&threads_complete_mutex);
  threads_complete++;
  if(aJob->typeofjob == test){
    myScheduler->test_batches++;
  }
  destroy_queue_element(&aJob);
  pthread_mutex_unlock(&threads_complete_mutex);
  pthread_cond_signal(&are_calcs_done_cond, &threads_complete_mutex);  
}


int execute_all_jobs(JobScheduler* sch){
  for(int i = 0; i < sch->execution_threads; i++){
    pthread_create(sch->tids[i], NULL, threadWork, (void*)sch );
  }
  
  pthread_mutex_lock(&threads_complete_mutex); 
  while(threads_complete != execution_threads){
    pthread_cond_wait(&are_calcs_done_cond, &threads_complete_mutex); 
  }
  time_to_work = 0;
  if(read_from_coeff_array == 1){
    double coefficients_temp[COEFF_AMOUNT];
    // calculate new coefficients (still in training phase)
    for(int i = 0; i < COEFF_AMOUNT; i++){
      for(int j = 0; j < sch->execution_threads; j++){
        coefficients_temp[i] += threads_coeffs[i][j];
      }
      coefficients_temp[i] = coefficients_temp[i]/((double)sch->execution_threads);
      coefficients_temp[i] *= 0.1; // learning rate
    }
    for(int i = 0; i < COEFF_AMOUNT; i++){
      coefficients[i] = coefficients[i] - coefficients_temp[i];
    }
  }else{
    for(int i = 0; i < sch->execution_threads; i++){
      all_correct_predictions += thread_correct_predictions[i];
    }
  }
  threads_complete = 0;
  pthread_mutex_unlock(&threads_complete_mutex); 
  pthread_cond_signal(&are_calcs_done_cond, &threads_complete_mutex);
  time_to_work = 1;
  pthread_cond_signal(&can_i_take_a_job, &queue_mutex);
  
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
