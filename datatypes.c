#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"

void create_node(list_node* node, char* temp_id){
    node->id = malloc((strlen(temp_id) + 1)*sizeof(char));
    strcpy(node->id, temp_id);
}

void delete_node(list_node* node){
    free(node->id);
    //printf("node->id freed\n");
}