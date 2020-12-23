#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"
//#include "set.h"
#include "negcl.h"


void neglist_create(list_node** lnode){
    (*lnode)->ngl = malloc(sizeof(neg_list));
    (*lnode)->ngl->front = NULL;
    (*lnode)->ngl->rear = NULL;
    // printf("Behold my contents: nothing\n");
    
    // (*lnode)->ngl->front->neg_clique == NULL;
    // (*lnode)->ngl->rear->neg_clique == NULL;
}

neg_node* create_negnode(list_node* cld){
    neg_node* new_node = malloc(sizeof(neg_node));
    // new_node->neg_clique = malloc(sizeof(clique_list_node));
    new_node->neg_clique = cld;
    new_node->next_in_negclique = NULL;
    new_node->printed = 1;
    return new_node;
}

void neglist_add(/*neg_list* neglist, */list_node* repres01, list_node* repres02){
    //neglist_add is based on 3 cases
    // if(repres01->ngl->front->neg_clique == NULL){
    // printf("Behold my contents: %s\t%s\n",repres01->id,repres02->id);
    if(repres01->ngl->front== NULL){
        // case 1: repres01's neglist is empty and repres02 will be added there 
        // => repres02's neglist doesn't have repres01 in it so it must be added
        // neglist_create(&repres01);
        repres01->ngl->front = create_negnode(repres02); 
        repres01->ngl->rear = repres01->ngl->front;
        //adding repres01 to repres02 neglist
        if(repres02->ngl->front== NULL){
            // case 1.1: repres02's neglist is also empty
            // neglist_create(&repres02);
            repres02->ngl->front = create_negnode(repres01);
            repres02->ngl->rear = repres02->ngl->front;
        }else{
            repres02->ngl->rear->next_in_negclique = create_negnode(repres01);
            repres02->ngl->rear = repres02->ngl->rear->next_in_negclique;
        }
    }else{
        neg_node *temp;
        temp = repres01->ngl->front;
        int flag = 0; //we assume that repres02 isn't included in repres01's neglist 
        while(temp != NULL){//->next_in_negclique != NULL){
            if(strcmp(temp->neg_clique->id,repres02->id)==0){
                flag = 1;
                break;
                // no need to check repres02 since both are "connected"
            }
            temp = temp->next_in_negclique;
        }
        if(flag == 0){
            temp = repres01->ngl->rear;
            // case 2: repres01 neglist doesn't have repres02
            // adding repres02 to neglist
            temp->next_in_negclique = create_negnode(repres02);
            repres01->ngl->rear = temp->next_in_negclique;
            // If repres01 neglist didn't have repres02 then
            // the same must apply and vice versa 
            if(repres02->ngl->front== NULL){
                // neglist_create(&repres02);
                repres02->ngl->front = create_negnode(repres01);
                repres02->ngl->rear = repres02->ngl->front;
            }else{
                repres02->ngl->rear->next_in_negclique = create_negnode(repres01);
                repres02->ngl->rear = repres02->ngl->rear->next_in_negclique;                
            }
        }
    }
}

void neglist_print(list_node* repres){
    list_node *rep_temp = repres;
    neg_node* temp;
    if(repres->ngl!=NULL){
        temp = repres->ngl->front;
    }else{
        printf("Empty: \t%s\n",repres->id);
        temp = NULL;
    }
    list_node *neg_temp;
    while(temp){
        rep_temp = repres;
        neg_temp = temp->neg_clique;
        
        if(temp->printed){
            neg_node* sernode = neg_temp->ngl->front;
            while(sernode){//searching for repres in neg_temp's ngl to avoid printing duplicates
                if(strcmp(sernode->neg_clique->id,repres->id)==0){
                    sernode->printed = 0;
                    break;
                }
                sernode = sernode->next_in_negclique;
            }
            
            ///
            while(rep_temp){
                while(neg_temp){
                    printf("%s, %s, 0\n", rep_temp->id, neg_temp->id);
                    neg_temp = neg_temp->next_in_clique;
                }
                rep_temp = rep_temp->next_in_clique;
                neg_temp = temp->neg_clique;
            }
        }
        temp = temp->next_in_negclique;
    }
    //printf("printed all neg list representatives of %s\n",repres->id);
}

void delete_negnode(neg_node** negptr){
    int error;
    free((*negptr)->neg_clique);
    (*negptr)->next_in_negclique = NULL;
    free(*negptr);
    *negptr = NULL;
}

void destroy_neglist(list_node** repres){
    neg_node* temp = (*repres)->ngl->front;
    if(temp != NULL && (*repres)->ngl->rear != NULL){
        neg_node* next = NULL;
        while(temp){
            if(temp->next_in_negclique){
                next = temp->next_in_negclique;
                temp->next_in_negclique = NULL;
            }
            delete_negnode(&temp);
            temp = next;
        }
    }
    (*repres)->ngl->front = NULL;
    (*repres)->ngl->rear = NULL;
    free((*repres)->ngl);
    (*repres)->ngl = NULL;
}