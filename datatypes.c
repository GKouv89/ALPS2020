#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"
#include "tuplist.h"

void list_create(info_list** list){
    (*list) = malloc(sizeof(info_list));
    (*list)->front = NULL;
    (*list)->rear = NULL;
}

list_node* create_node(char* temp_id){
    list_node* new_node = malloc(sizeof(list_node));
    new_node->id = malloc((strlen(temp_id) + 1)*sizeof(char));
    strcpy(new_node->id, temp_id);
    new_node->content = NULL;
    new_node->previous = new_node->next = new_node->parent = new_node->next_in_clique = NULL;
    new_node->amount = 1;
    return new_node;
}

void list_add(info_list* list, list_node* node){
    if(list->front == NULL){ // empty list
        list->front = node;
        list->rear = node;
        node->previous = NULL;
        node->next = NULL;
    }else{
        node->previous = list->rear;
        node->next = NULL;
        list->rear->next = node;
        list->rear = node;
    }
}

void list_print(info_list* list){
    list_node* temp = list->front;
    while(temp){
        printf("%s\n", temp->id);
        tuplist_print(&(temp->content));
        temp = temp->next;
    }
}

void destroy_list(info_list** list){
    list_node* temp = (*list)->rear;
    if(temp != NULL && (*list)->front != NULL){
        list_node* prev = NULL;
        while(temp){
            prev = temp->previous;
            if(prev){ // Otherwise, we've reached list front
                prev->next = NULL;
            }
            delete_node(&temp);
            temp = prev;
        }            
    }
    (*list)->front = NULL;
    (*list)->rear = NULL;
    free(*list);
    *list = NULL;
}

void delete_node(list_node** node){
    int error;
    free((*node)->id);
    tuplist_destroy(&((*node)->content), &error);
    (*node)->next = NULL;
    (*node)->previous = NULL;
    free(*node);
    *node = NULL;
    //printf("node->id freed\n");
}
