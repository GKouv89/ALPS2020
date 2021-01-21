struct Job{
  int job_id;
  (void *)routine; //this is supposedly a function pointer
  FILE *fp; // data on which the routine will be performed on
  int thread_id;
}


JobScheduler* initialize_scheduler(int execution_threads){
  JobScheduler new = allocate memory for scheduler;
  new->execution_threads = execution_threads;
  new->threads_complete = 0;
  // create job queue
  new->tids = create an array of execution_threads+1 elements
  the additional element will be the main thread
  new->mutex = PTHREAD_MUTEX_INITIALIZER;
  new->cond = PTHREAD_COND_INITIALIZER;
}

int submit_job(JobScheduler* sch, Job* j){
  // this function is responsible for adding jobs to the queue
  // will be called multiple times from main
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
  // destroy pthread tids
  // destroy scheduler itself
}