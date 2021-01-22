#include "queueelement.h"
#include <stdlib.h>
#include <string.h>

void create_queue_element(qelem **new_elem, typeofjob type, char *file_name){
  static int job_id_counter = 0;
  (*new_elem) = malloc(sizeof(new_elem));
  (*new_elem)->job_id = job_id_counter;
  (*new_elem)->type = type;
  (*new_elem)->file_name = malloc(strlen(file_name) + 1);
  strcpy((*new_elem)->file_name, file_name);
  job_id_counter++;
}

void destroy_queue_element(qelem **elem){
  free((*elem)->file_name);
  (*elem)->file_name = NULL;
  free(*elem);
  *elem = NULL;
}
