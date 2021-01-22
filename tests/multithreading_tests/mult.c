#include "../acutest.h"
#include "../../multithreading/scheduler.h"

struct tfidf_vector{
  double value[7];
};

struct tfidf_vector tfidf_array[] = {
  [0].value[0] = 0.04875, 
  [0].value[0] = 0, 
  [0].value[0] = 0, 
  [0].value[0] = 0, 
  [0].value[0] = 0, 
  [0].value[0] = 0, 
  [0].value[0] = 0, 
  [1].value[1] = 0.0624, 
  [1].value[1] = 0.0624, 
  [1].value[1] = 0, 
  [1].value[1] = 0, 
  [1].value[1] = 0, 
  [1].value[1] = 0, 
  [1].value[1] = 0, 
  [2].value[2] = 0, 
  [2].value[2] = 0, 
  [2].value[2] = 0.0624, 
  [2].value[2] = 0.0624, 
  [2].value[2] = 0, 
  [2].value[2] = 0, 
  [2].value[2] = 0, 
  [3].value[3] = 0, 
  [3].value[3] = 0.0624, 
  [3].value[3] = 0, 
  [3].value[3] = 0.0624, 
  [3].value[3] = 0.0624, 
  [3].value[3] = 0, 
  [3].value[3] = 0, 
  [4].value[4] = 0, 
  [4].value[4] = 0, 
  [4].value[4] = 0.0975, 
  [4].value[4] = 0, 
  [4].value[4] = 0.0975, 
  [4].value[4] = 0.1725, 
  [4].value[4] = 0.1725
};

void test_scheduler_creation(){
  JobScheduler *sch = initialize_scheduler(5, NULL, NULL);
}

TEST_LIST = {
  {"scheduler_creation", test_scheduler_creation},
  {NULL, NULL}
};

