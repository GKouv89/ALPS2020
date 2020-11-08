#ifndef __HASHMAPS_H__
#define __HASHMAPS_H__

#include "datatypes.h"
#define BUCKETS 997


typedef struct bucket{
    int id;
    int no_of_entries; // this is mostly necessary for evaluation of hash function
    info_list* bucket_list;
}hash_bucket;

typedef struct map{
    hash_bucket map[BUCKETS];
}hash_map;

hash_map* create_map();
void add_to_bucket(hash_map*, int, list_node*);
void destroy_map(hash_map**);
int hash_function(hash_map*, const char*);
void print_bucket_no_of_entries(hash_map*);
#endif