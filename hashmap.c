#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "datatypes.h"

hash_map* create_map(){
    hash_map* map = (hash_map *)malloc(sizeof(hash_map));
    for(int i = 0; i < BUCKETS; i++){
        //map->map[i] = (hash_bucket *)malloc(sizeof(hash_bucket));
        map->map[i].id = i;
        map->map[i].no_of_entries = 0;
        list_create(&(map->map[i].bucket_list));
    }
    return map;
}

int hash_function(hash_map* map, const char* id){ 
    unsigned int hash = 7;
    for(int i = 0; i < strlen(id); i++){
        hash = hash * 31 + id[i];
    }
    
    hash = hash % BUCKETS;
    return hash;
}

void add_to_bucket(hash_map* map, int bucket, list_node* node){
    list_add(map->map[bucket].bucket_list, node);
    map->map[bucket].no_of_entries++;
}


void print_bucket_no_of_entries(hash_map* map){
    int median = 0;
    int max = 0;
    int min = 500;
    int empty_buckets = 0;
    for(int i = 0; i < BUCKETS; i++){
        printf("Bucket no %d has %d entries\n", i, map->map[i].no_of_entries);
        median += map->map[i].no_of_entries;
        if(map->map[i].no_of_entries < min){
            min = map->map[i].no_of_entries;
        }
        if(map->map[i].no_of_entries > max){
            max = map->map[i].no_of_entries;
        }
        if(!map->map[i].no_of_entries){
            empty_buckets++;
        }
    }
    median = median/BUCKETS;
    printf("On average, each bucket contains %d entries.\n", median);
    printf("Max no of entries in bucket is %d. Min no is %d.\n", max, min);
    printf("We have %d empty_buckets.\n", empty_buckets);
}

void destroy_map(hash_map** map){
    for(int i = 0; i < BUCKETS; i++){
        destroy_list(&((*map)->map[i].bucket_list));
    }
    free(*map);
}
