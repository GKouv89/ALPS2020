#ifndef QUEUEELEMENT_H
#define QUEUEELEMENT_H

#include <pthread.h>

typedef struct Job{
  int job_id;
  void (*routine)(void *); //this is supposedly a function pointer
  char *file_name; // data on which the routine will be performed on
  p_thread_t thread_id;
}qelem;

void create_queue_element(qelem **, void *(void *), char *);
void destroy_queue_element(qelem **);
#endif
