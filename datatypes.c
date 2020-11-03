#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"

list_node* create_node(char* temp_id){
    list_node* new_node = malloc(sizeof(list_node));
    new_node->id = malloc((strlen(temp_id) + 1)*sizeof(char));
    strcpy(new_node->id, temp_id);
    return new_node;
}

void delete_node(list_node** node){
    free((*node)->id);
    free(*node);
    *node = NULL;
    //printf("node->id freed\n");
}