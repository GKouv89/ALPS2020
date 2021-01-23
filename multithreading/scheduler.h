#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <pthread.h>
#include <unistd.h>
#include "../TF-IDF/tf.h"
#include "../hashmap.h"
#include "routines.h"
#include "queueelement.h"
#include "queue.h"

#ifndef COEFFAMOUNT
  #define COEFFAMOUNT 15
#endif

typedef struct scheduler{
  int execution_threads;
  int *time_to_work; // boolean values, indicate if thread[i] can take job from pool
  int threads_complete; // number of threads that have finished execution in current iteration
  int read_from_coeff_array; // boolean value, shows what type of result we expect from threads (coefficients or accuracy?)
  // Common data for all threads //
  hash_map* map;
  tf* tf_array;
  /////////////////////////////////
  queue* q; // job queue
  pthread_t* tids;
  pthread_cond_t are_calcs_done_cond;
  pthread_cond_t can_i_take_a_job;
  pthread_mutex_t queue_mutex;
  pthread_mutex_t threads_complete_mutex;
  pthread_mutex_t can_i_take_a_job_mutex;
  // Common structures for all threads/jobs to write their results in/read stuff from.
  double coefficients[COEFFAMOUNT]; // the common coefficients that every thread *starts* with
  /* thread_predictions: array, one element per thread
  when breaking down a batch into pieces, each thread writes in thread_predictions 
  how many lines the batch piece it took on had,
  so if a batch is smaller than average batch size (for example, the last batch)
  and its pieces don't all have the same number of lines
  we are still able to correctly calculate overall accuracy/new coefficients (used in both) */
  int *thread_predictions;
  /* threads_coeffs contains the sum of (sigmoid - ground truth)*x for all lines a thread has read
  when all threads have calculated this for a part of a batch, the master thread will sum
  every element of threads_coeffs and will then divide its elements by thread_predictions.
  This is the average loss. Then we multiply with the learning rate and update the coefficients. */
  double (*threads_coeffs)[COEFFAMOUNT];  
  int *thread_correct_predictions; // needs to be allocated execution_threads amount of columns
  int all_correct_predictions; // sum of thread_correct_predictions' elements per execution_threads no. of batches
  int all_predictions_testing; // sum of thread_predictions during testing phase, to be able to correctly calculate 
  // overall accuracy when all testing batches are done
}JobScheduler;

JobScheduler* initialize_scheduler(int, hash_map*, tf*);
int submit_job(JobScheduler*, qelem*);
int execute_all_jobs(JobScheduler*);
int destroy_scheduler(JobScheduler*);
#endif