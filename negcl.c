#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"
//#include "set.h"
#include "negcl.h"


void neglist_create(neg_list** ngl){
    (*ngl) = malloc(sizeof(neg_list));
    (*ngl)->front = NULL;
}

neg_node* create_negnode(list_node* cld){
    neg_node* new_node = malloc(sizeof(neg_node));
    // new_node->neg_clique = malloc(sizeof(clique_list_node));
    new_node->neg_clique = cld;
    new_node->next_in_negclique = NULL;
    return new_node;
}

void neglist_add(/*neg_list* neglist, */list_node* repres01, list_node* repres02){
    //neglist_add is based on 3 cases
    if(repres01->ngl->front->neg_clique == NULL){
        // case 1: repres01's neglist is empty and repres02 will be added there 
        // => repres02's neglist doesn't have repres01 in it so it must be added 
        repres01->ngl->front = create_negnode(repres02); 
        repres01->ngl->rear = repres01->ngl->front;
        //adding repres01 to repres02 neglist
        if(repres02->ngl->front->neg_clique == NULL){
            repres02->ngl->front = create_negnode(repres01);
            repres02->ngl->rear = repres02->ngl->front;
        }else{
            repres02->ngl->rear->next_in_negclique = create_negnode(repres01);
            repres02->ngl->rear = repres02->ngl->rear->next_in_negclique;
            // neg_node *temp=malloc(sizeof(neg_node));
            // temp=repres02->ngl->front;
            // while(temp->next_in_negclique != NULL){
                // temp = temp->next_in_negclique;
            // }
            // temp->next_in_negclique = create_node(repres01);
        }
    }else{
        neg_node *temp;
        temp=repres01->ngl->front;
        int flag=0; //we assume that repres02 isn't included in repres01's neglist 
        while(temp->next_in_negclique != NULL){
            if(strcmp(temp->neg_clique->id,repres02->id)==0){
                flag=1;
                break;
                // no need to check repres02 since both are "connected"
            }
            temp = temp->next_in_negclique;
        }
        if(flag==0){
            // case 2: repres01 neglist doesn't have repres02
            // adding repres02 to neglist
            temp->next_in_negclique = create_negnode(repres02);
            repres01->ngl->rear = temp->next_in_negclique;
            // If repres01 neglist didn't have repres02 then
            // the same must apply and vice versa 
            repres02->ngl->rear->next_in_negclique = create_negnode(repres01);
            repres02->ngl->rear = repres02->ngl->rear->next_in_negclique;
        }
    }
}

void neglist_print(neg_list* negli){
    
}

void delete_negnode(neg_node** negliptr){
    
}

void destroy_neglist(neg_list** negliptr){
    
}