#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "datatypes.h"
#include "parse.h"
#include "hashmap.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    hash_map* map = create_map();
    printf("hash map created\n");
    parser(map);
    print_bucket_no_of_entries(map);
    destroy_map(&map);
    printf("hash map destroyed\n");
}