#include <pthreads.h>
#include "../TF-IDF/tf.h"
#include "../hashmap.h"

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
}JobScheduler;