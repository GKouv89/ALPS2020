#ifndef QUEUEELEMENT_H
#define QUEUEELEMENT_H

#include <pthread.h>

typedef enum type {training, validation, testing} typeofjob;

typedef struct Job{
  int job_id;
  // void (*routine)(void *); //this is supposedly a function pointer
  typeofjob type; 
  char *file_name; // data on which the routine will be performed on
  pthread_t thread_id;
}qelem;

void create_queue_element(qelem **, typeofjob, char *);
void destroy_queue_element(qelem **);
#endif