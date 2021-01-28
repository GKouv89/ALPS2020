#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"
#include "tuplist.h"
#include "negcl.h"

void list_create(info_list** list){
    (*list) = malloc(sizeof(info_list));
    (*list)->front = NULL;
    (*list)->rear = NULL;
}

list_node* create_node(char* temp_id, int vecNum){
    list_node* new_node = malloc(sizeof(list_node));
    new_node->id = malloc((strlen(temp_id) + 1)*sizeof(char));
    strcpy(new_node->id, temp_id);
    new_node->vec_num = vecNum;
    new_node->content = NULL;
    new_node->ngl = NULL;
    new_node->matches = NULL;
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

void normalize_preds(info_list *list, double max_pred){
  list_node *temp = list->front;
  while(temp){
    normalize(temp, max_pred);
    temp = temp->next;
  }
}

void reinitialize_cliques(info_list* list){
  list_node *temp = list->front;
  while(temp){
    temp->amount = 1;
    temp->parent = NULL;
    temp->next_in_clique = NULL;
    destroy_neglist(&temp);
    destroy_matchlist(&temp);
    temp = temp->next;
  }
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
