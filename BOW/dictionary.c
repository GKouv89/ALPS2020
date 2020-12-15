#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dictionary.h"

void create_tree(tree_node **tree){
	*tree=NULL;
}

int isEmpty_tree(tree_node *tree){
 	return (tree==NULL);
}


void destroy_tree(tree_node **tree){
	if(isEmpty_tree(*tree)){
    	return;
    }
	
	destroy_tree(&(*tree)->left_child);
	destroy_tree(&(*tree)->left_child);
    free((*tree)->node_cont.word);
	free(*tree);
	*tree=NULL;
}

void insert_tree(tree_node **tree, char *word, int payload, int *error){
    if(isEmpty_tree(*tree)){
		*tree = malloc(sizeof(tree_node));
		assert(*tree != NULL);
        (*tree)->node_cont.word = malloc(strlen(word) + 1);
        strcpy((*tree)->node_cont.word, word);
        (*tree)->node_cont.payload = payload;
		(*tree)->left_child = NULL;
		(*tree)->right_child = NULL;
        *error = 0;
	}else if(strcmp(word, (*tree)->node_cont.word) < 0){
        insert_tree(&((*tree)->left_child), word, payload, error);
    }else if(strcmp(word, (*tree)->node_cont.word) > 0){
        insert_tree(&((*tree)->right_child), word, payload, error);
    }else{
        *error=1;
    }
}

int search_tree(tree_node *tree, char *word){
    tree_node *temp = tree;
    if(isEmpty_tree(temp)){
		return -1;
	}
	if(strcmp(temp->node_cont.word, word) == 0){
		return temp->node_cont.payload;
	}else if(strcmp(temp->node_cont.word, word) > 0){
    	search_tree(temp->left_child, word);
    }else if(strcmp(temp->node_cont.word, word) < 0){
    	search_tree(temp->right_child, word);
    }
}

void print_tree(tree_node *tree){
    tree_node *temp = tree;
    if(!isEmpty_tree(temp)){
        print_tree(temp->left_child);
        printf("word: %s\tpayload: %d\n", temp->node_cont.word, temp->node_cont.payload);
        print_tree(temp->right_child);
	}
}