#include <stdio.h>
#include <stdlib.h>

#include "set.h"
#include "hashmap.h"

void create_clique_list(clique_list** list){
    (*list) = malloc(sizeof(clique_list));
    (*list)->front = (*list)->rear = NULL;
}

void insert_master(clique_list* list, list_node* a){
    clique_list_node *temp = list->front; 
    while(temp){
        if(temp->representative == a){
            return; // CLIQUE REPRESENTATIVE ALREADY IN LIST
        }
        temp = temp->next;
    }
    // if representative not in list, we proceed with insertion
    clique_list_node* new = malloc(sizeof(clique_list_node));
    new->representative = a;
    if(list->front == NULL){
        new->prev = new->next = NULL;
        list->front = new;
        list->rear = new;
    }else{
        new->prev = list->rear;    
        new->next = NULL;
        list->rear->next = new;
        list->rear = new;
    }
    
}

void remove_master(clique_list* list, list_node* a){
    clique_list_node* temp = list->front;
    while(temp){
        if(temp->representative == a){
            if((temp == list->front) && (temp == (list->rear))){ // no other nodes after removal
                list->front = list->rear = NULL;
            }else if(temp == list->front){ // removing first node, list->front goes one position to the right
                temp->next->prev = NULL;
                list->front = temp->next;
            }else if(temp == list->rear){ // removing last node, list->rear goes one position to the left
                temp->prev->next = NULL;
                list->rear = temp->prev;
            }else{ // there's both a next and prev node
               temp->prev->next = temp->next;
               temp->next->prev = temp->prev;  
            }
            temp->prev = temp->next = NULL;
            temp->representative = NULL;
            free(temp);
            return;
        }
        temp = temp->next;
    }
}

list_node* find_root(list_node* a){
    list_node* result = a;
    while(result->parent != NULL){
        result = result->parent;
    }
    return result;
}

list_node* join_sets(clique_list* l, list_node* a, list_node* b){
    list_node *root_a = find_root(a);
    list_node *root_b = find_root(b);
    list_node *temp;
    if(root_a == root_b){
        return root_a;
    }else{
        int max_amount = root_a->amount;
        int flag = 0; // root_a will be the new root
        if(root_b->amount > max_amount){
            flag = 1;
        }
        if(flag){ // root_b is the new root
            root_a->parent = root_b;
            root_b->amount += root_a->amount;
            root_a->amount = -1;
            temp = root_b;
            while(temp->next_in_clique != NULL){
                temp = temp->next_in_clique;
            }
            temp->next_in_clique = root_a; // Connecting the former cliques lists, to be ready for printing
            remove_master(l, root_a); // If we are just starting to form the cliques,
            // then root_a might be 'a' itself. When a node is a clique by itself, then it is not
            // included in clique_list (otherwise we'd have ~30.000 cliques at first). In this case,
            // remove_master just won't do anything.
            insert_master(l, root_b); // If root_b already in list, he won't be added again
            return root_b;
        }else{
            root_b->parent = root_a;
            root_a->amount += root_b->amount;
            root_b->amount = -1;
            temp = root_a;
            while(temp->next_in_clique != NULL){
                temp = temp->next_in_clique;
            }
            temp->next_in_clique = root_b;
            remove_master(l, root_b);
            insert_master(l, root_a);
            return root_a;
        }
    }
}

void print_all_cliques(int opt, clique_list* l){
    clique_list_node *temp = l->front;
    while(temp){
        // printf("\n");
        print_clique(opt, temp->representative);
        temp = temp->next;
    }
}

void print_clique(int opt, list_node *root){
    // if(!opt){
        list_node *temp = root;
        list_node *temp_2;
        while(temp != NULL){
            temp_2 = temp->next_in_clique;
            while(temp_2 != NULL){
                printf("%s, %s\n", temp->id, temp_2->id);
                temp_2 = temp_2->next_in_clique;
            }
            temp = temp->next_in_clique;
        }
    // }else{
        // FILE* fp = fopen("clique.txt", "w");
        // list_node *temp = root;
        // list_node *temp_2;
        // while(temp != NULL){
            // temp_2 = temp->next_in_clique;
            // while(temp_2 != NULL){
                // fprintf(fp, "%s, %s\n", temp->id, temp_2->id);
                // temp_2 = temp_2->next_in_clique;
            // }
            // temp = temp->next_in_clique;
        // }
        // fclose(fp);
    // }
}

void delete_clique_node(clique_list_node** node){
    (*node)->representative = NULL;
    (*node)->prev = (*node)->next = NULL;
    free(*node);
    *node = NULL;
}

void destroy_clique_list(clique_list **l){
    clique_list_node *temp = (*l)->rear;
    if(temp != NULL && (*l)->front != NULL){
        clique_list_node *prev = NULL;
        while(temp){
            prev = temp->prev;
            if(prev){ // Otherwise, we've reached list front
                prev->next = NULL;
            }
            delete_clique_node(&temp);
            temp = prev;
        }
    }
    (*l)->front = NULL;
    (*l)->rear = NULL;
    free(*l);
    *l = NULL;
}