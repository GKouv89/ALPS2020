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
  lock queue_mutex
  while(time_to_work == 0){
    pthread_cond_wait(&can_i_take_a_job, &queue_mutex); 
  }
  if(queue is not empty){    
    take first job from queue;
  }
  unlock queue_mutex
  cond_var_signal(&can_i_take_a_job, &queue_mutex);

  run job
  store result in suitable place, depending on if training or testing
  lock threads_complete_mutex
  threads_complete++
  unlock threads_complete_mutex
  cond_var_signal(&are_calcs_done_cond, &threads_complete_mutex);
  
}


int execute_all_jobs(JobScheduler* sch){
  create all threads;
  
  lock threads_complete_mutex 
  while(threads_complete != execution_threads){
    pthread_cond_wait(&are_calcs_done_cond, &threads_complete_mutex); 
  }
  time_to_work = 0;
  if(read_from_coeff_array == 1){
    // calculate new coefficients (still in training phase)
  }else{
    // calculate overall accuracy
  }
  threads_complete = 0;
  unlock threads_complete_mutex 
  cond_var_signal(&are_calcs_done_cond, &threads_complete_mutex);
  time_to_work = 1;
  cond_var_signal(&can_i_take_a_job, &queue_mutex);
  
}

int destroy_scheduler(JobScheduler* sch){
  // destroy cond vars and semaphores - is it necessary before thread destruction?
  // destroy all threads
  // destroy queue
  queue_destroy(&(sch->q));
  // destroy pthread tids
  // destroy scheduler itself
}


void decrement(int batchsize){
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

// int main(int argc, char* argv[]){
    // FILE* fp = fopen("TrainingSet_medium.csv", "r");
    // char c;
    // int batchsize = 512;
    // int count = 0;
    // for (c = getc(fp); c != EOF; c = getc(fp)){
        // if (c == '\n'){ // Increment count if this character is newline 
            // count = count + 1; 
        // }
    // }
    // // Close the file 
    // filenum = (28122/batchsize) +1;
    // printf("Num of lines: %d\n%d\n", count, filenum);
    // fclose(fp); 
// }