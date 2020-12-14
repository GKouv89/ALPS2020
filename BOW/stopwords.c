#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stopwords.h"

void make_stopword_list(sw_list **l){
    FILE *fp = fopen("common-english-words.txt", "r");
    assert(fp != NULL);
    
    size_t line_size = 50;
    char *line = malloc(line_size*sizeof(char));
    int bytes_read;
    char *temptok;
    
    create(l);
    assert((*l) != NULL);
    
    while((bytes_read = getdelim(&line, &line_size, ',', fp)) != -1){
        temptok = strtok(line, ",");
        insert((*l), temptok);
    }
    
    assert(fclose(fp) == 0);
}

void create(sw_list **l){
    *l = malloc(sizeof(sw_list));
    (*l)->front = NULL;
    (*l)->back = NULL;
}

void insert(sw_list *l, char *word){
    sw_node *node = malloc(sizeof(sw_node));
    node->word = malloc((strlen(word) + 1)*sizeof(char));
    strcpy(node->word, word);
    node->next = NULL;
    if(l->front == NULL && l->back == NULL){
        l->front = node;
        l->back = node;
    }else{
        l->back->next = node;
        l->back = node;
    }
}

int is_stopword(sw_list *l, char *word){
    sw_node *temp = l->front;
    while(temp){
        if(strcmp(word, temp->word) == 0){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void print_sw(sw_list *l){
    sw_node *temp = l->front;
    while(temp){
        printf("%s\n", temp->word);
        temp = temp->next;
    }
}

void destroy(sw_list **l){
    sw_node *temp = (*l)->front;
    sw_node *to_del;
    while(temp){
        to_del = temp;
        temp = temp->next;
        free(to_del->word);
        free(to_del);
        to_del = NULL;
    }
    free(*l);
    (*l) = NULL;
}