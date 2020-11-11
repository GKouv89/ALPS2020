#include <stdio.h>
#include <stdlib.h>

#include "set.h"
#include "hashmap.h"

list_node* find_root(list_node* a){
    list_node* result = a;
    while(result->parent != NULL){
        result = result->parent;
    }
    return result;
}

list_node* join_sets(list_node* a, list_node* b){
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
            temp->next_in_clique = a;
            return root_b;
        }else{
            root_b->parent = root_a;
            root_a->amount += root_b->amount;
            root_b->amount = -1;
            temp = root_a;
            while(temp->next_in_clique != NULL){
                temp = temp->next_in_clique;
            }
            temp->next_in_clique = b;
            return root_a;
        }
    }
}

void print_clique(int opt, list_node *root){
    if(!opt){
        list_node *temp = root;
        // printf("nodes in clique: %d\n", root->amount);
        list_node *temp_2;
        while(temp != NULL){
            temp_2 = temp->next_in_clique;
            while(temp_2 != NULL){
                printf("%s, %s\n", temp->id, temp_2->id);
                temp_2 = temp_2->next_in_clique;
            }
            temp = temp->next_in_clique;
        }
    }else{
        FILE* fp = fopen("clique.txt", "w");
        list_node *temp = root;
        list_node *temp_2;
        while(temp != NULL){
            temp_2 = temp->next_in_clique;
            while(temp_2 != NULL){
                fprintf(fp, "%s, %s\n", temp->id, temp_2->id);
                temp_2 = temp_2->next_in_clique;
            }
            temp = temp->next_in_clique;
        }
        fclose(fp);
    }
}