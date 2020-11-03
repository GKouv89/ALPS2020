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
        map->map[i]. bucket_list = NULL;
    }
    return map;
}

void hash_function(hash_map* map, const char* id){
    // choose at random 5 positions
    int pos[5], sum = 0;
    char letters[5];
    for(int i = 0; i < 5; i++){
        pos[i] = rand() % (strlen(id) + 1);
        letters[i] = id[pos[i]];
    }
    for(int i = 0; i < 5; i++){
        sum += letters[i];
    }
    sum = sum % BUCKETS;
    map->map[sum].no_of_entries++;
}

void print_bucket_no_of_entries(hash_map* map){
    for(int i = 0; i < BUCKETS; i++){
        printf("Bucket no %d has %d entries\n", i, map->map[i].no_of_entries);
    }
}

void destroy_map(hash_map** map){
    free(*map);
}