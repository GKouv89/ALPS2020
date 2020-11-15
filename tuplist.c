#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tuplist.h"
#include "datatypes.h"

int list_empty(tuplelist *tl){
	return(tl->head == NULL);
}

void list_create(tuplelist **tl, int *error){
    if (*tl) {
		*error = 1;
	} else {
		*error = 0;      
		*tl = malloc(sizeof(tuplelist));
        (*tl)-> head = NULL;
        (*tl)-> last = NULL;
	}
}

node_tuple* list_create_node(char* buffname, char* buffval){
    node_tuple* node = malloc(sizeof(node));
    node->name = malloc(strlen(buffname)+1); //allocating buffer size + 1
    strcpy(node->name, buffname); //coping atr name from buffer to node
    node->value = malloc(strlen(buffval)+1);
    strcpy(node->value, buffval);  //coping atr val from buffer to node
	node->next = NULL;
	return node;
}

void list_insert(tuplelist **tl, char* buffname, char* buffval){
    node_tuple* temp;
    if(!list_empty(*tl)){
        temp = (*tl)->head;
        while(temp->next!=NULL){
            temp = temp-> next;
        }
        temp->next = list_create_node(buffname, buffval);
        (*tl)->last = temp->next;
    }else{
        temp = list_create_node(buffname, buffval);
        (*tl)->head = temp;
        (*tl)->last = temp;
    }
    while(temp->next!=NULL){
        temp = temp-> next;
    }
    temp->next = list_create_node(buffname, buffval);
}

void list_destroy_node(node_tuple **tn){
	free(*tn);
	*tn = NULL;
}

void list_destroy(tuplelist **tl, int *error){
	tuplelist* ltemp;
    node_tuple* temp;
    node_tuple* todel;
	if (*tl == NULL) {
		*error = 1;
	} else {
        ltemp = *tl;
        temp = ltemp->head;
		*error = 0;
		while (temp) {
            todel = temp;
            temp = temp ->next;
			list_destroy_node(&todel);
		}
        free(*tl);
        *tl = NULL;
	}
}