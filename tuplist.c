#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tuplist.h"
#include "datatypes.h"

int tuplist_empty(tuplelist *tl){
	return(tl->head == NULL);
}

void tuplist_create(tuplelist **tl, int *error){
    if (*tl) {
		*error = 1;
	} else {
		*error = 0;      
		*tl = malloc(sizeof(tuplelist));
        (*tl)-> head = NULL;
        (*tl)-> last = NULL;
	}
}

node_tuple* tuplist_create_node(char* buffname, char* buffval){
    node_tuple* node = malloc(sizeof(node_tuple));
    node->name = malloc(strlen(buffname)+1); //allocating buffer size + 1
    strcpy(node->name, buffname); //coping atr name from buffer to node
    node->value = malloc(strlen(buffval)+1);
    strcpy(node->value, buffval);  //coping atr val from buffer to node
	node->next = NULL;
	return node;
}

void tuplist_insert(tuplelist **tl, char* buffname, char* buffval){
    node_tuple* temp;
    if(!tuplist_empty(*tl)){
        /*temp = (*tl)->head;
        while(temp->next!=NULL){
            temp = temp-> next;
        }
        temp->next = tuplist_create_node(buffname, buffval);
        (*tl)->last = temp->next; */
	(*tl)->last->next = tuplist_create_node(buffname, buffval);
	(*tl)->last = (*tl)->last->next;
    }else{ 
        temp = tuplist_create_node(buffname, buffval);
        (*tl)->head = temp;
        (*tl)->last = temp;
    }
}

void tuplist_print(tuplelist **tl){
    node_tuple* temp = (*tl)->head;
    while(temp){
        printf("Attribute: %s\nValue: %s\n", temp->name, temp->value);
        temp = temp->next;
    }
}

void tuplist_destroy_node(node_tuple **tn){
	free(*tn);
	*tn = NULL;
}

void tuplist_destroy(tuplelist **tl, int *error){
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
			tuplist_destroy_node(&todel);
		}
        free(*tl);
        *tl = NULL;
	}
}
