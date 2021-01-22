#include <pthreads.h>
#include "../TF-IDF/tf.h"
#include "../hashmap.h"

#define COEFF_AMOUNT 2001

typedef struct scheduler{
  int execution_threads;
  int time_to_work; // boolean value, indicates threads can take job from pool
  int threads_complete; // number of threads that have finished execution in current iteration
  int read_from_coeff_array; // boolean value, shows what type of result we expect from threads (coefficients or accuracy?)
  hash_map* map;
  tf* tf_array;
  queue* q; // job queue
  pthread_t* tids;
  pthread_cond_t are_calcs_done_cond;
  pthread_cond_t can_i_take_a_job;
  pthread_mutex_t queue_mutex;
  pthread_mutex_t threads_complete_mutex;
  // Common structures for all threads/jobs.
  double coefficients[COEFF_AMOUNT];
  double threads_coeffs[][COEFF_AMOUNT];
  int *thread_correct_predictions; // needs to be allocated execution_threads amount of columns
  int all_correct_predictions; // sum of thread_correct_predictions' elements per execution_threads no. of batches
  int test_batches; // how many batches of testing we have done, will divide final value of all_correct_predictions
}JobScheduler;