#include "acutest.h"
#include "../hashmap.h"

void test_create(void){
    hash_map* map = create_map();
    TEST_CHECK(map != NULL);
    for(int i = 0; i < BUCKETS; i++){
       TEST_CHECK(map->map[i].id == i);
       TEST_CHECK(map->map[i].no_of_entries == 0);
       TEST_CHECK(map->map[i].bucket_list->front == NULL);
       TEST_CHECK(map->map[i].bucket_list->rear == NULL);
    }
    destroy_map(&map);
}

TEST_LIST = {
    { "create_hashmap", test_create },
    { NULL, NULL }
};

