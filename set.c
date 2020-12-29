#include <stdio.h>
#include <stdlib.h>

#include "set.h"
#include "hashmap.h"
#include "stdlib.h"

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
    clique_list_node *temp2 = l->front;

    FILE *fp1; // for positive relationship cliques
    FILE *fp2; // for negative relationship cliques
    FILE *fp3; // For random order results
    
    FILE *trainp; // Pointer to Training Set
    FILE *validp; // Pointer to Validation Set
    FILE *testp; // Pointer to Test Set
    
    // Preparing files for result splitting 
    fp1 = fopen("poscliques.csv","w+");
    fp2 = fopen("negcliques.csv","w+");
    fp3 = fopen("randresults.csv","w+");
    
    trainp = fopen("TrainingSet.csv","w+");
    validp = fopen("ValidationSet.csv","w+");
    testp = fopen("TestSet.csv","w+");
    
    if(fp1 == NULL){
        fprintf(stderr, "Couldn't open poscliques.csv file.\n");
    }
    if(fp2 == NULL){
        fprintf(stderr, "Couldn't open negcliques.csv file.\n");
    }
    if(fp3 == NULL){
        fprintf(stderr, "Couldn't randresults.csv file.\n");
    }
    
    printf("left_spec_id, right_spec_id\n");
    // if(opt==1){
        while(temp){
            // // printf("\n");
            print_clique(opt, temp->representative, fp1);
            temp = temp->next;
        }
    // }else{
        temp = l->front;
        while(temp){
            // // printf("\n");
            neglist_print(temp->representative, fp2);
            temp = temp->next;
        }
    // }
    //Creating random order results file
    int randomizer;
    double setnum;
    char *temps;
    size_t *n;
    temps = malloc(300*sizeof(char));
    fseek(fp1, 0, SEEK_SET);
    fseek(fp2, 0, SEEK_SET);
    randomizer = (rand()%2);
    while(feof(fp1)!=1 && feof(fp2)!=1){
        if(randomizer==1){
            getline(&temps,n,fp1);
            fprintf(fp3,"%s",temps);
            setnum =(double)rand()/(double)RAND_MAX;
            if(setnum<0.6){ //add line to training set
                fprintf(trainp,"%s",temps);
            }else if(0.6<=setnum && setnum<0.8){ //add line to validation set
                fprintf(validp,"%s",temps);
            }else{ //add line to test set
                fprintf(testp,"%s",temps);
            }
            free(temps);
            temps = malloc(300*sizeof(char));
            randomizer = (rand()%2);
        }else{
            getline(&temps,n,fp2);
            fprintf(fp3,"%s",temps);
            setnum =(double)rand()/(double)RAND_MAX;
            if(setnum<0.6){ //add line to training set
                fprintf(trainp,"%s",temps);
            }else if(0.6<=setnum && setnum<0.8){ //add line to validation set
                fprintf(validp,"%s",temps);
            }else{ //add line to test set
                fprintf(testp,"%s",temps);
            }
            free(temps);
            temps = malloc(300*sizeof(char));
            randomizer = (rand()%2);
        }
    }
    if(feof(fp1)){
        while(feof(fp2)!=1){
            getline(&temps,n,fp2);
            fprintf(fp3,"%s",temps);
            setnum =(double)rand()/(double)RAND_MAX;
            if(setnum<0.6){ //add line to training set
                fprintf(trainp,"%s",temps);
            }else if(0.6<=setnum && setnum<0.8){ //add line to validation set
                fprintf(validp,"%s",temps);
            }else{ //add line to test set
                fprintf(testp,"%s",temps);
            }
            free(temps);
            temps = malloc(300*sizeof(char));
            randomizer = (rand()%2);
        }
    }else if(feof(fp2)){
        while(feof(fp1)!=1){
            getline(&temps,n,fp1);
            fprintf(fp3,"%s",temps);
            setnum =(double)rand()/(double)RAND_MAX;
            if(setnum<0.6){ //add line to training set
                fprintf(trainp,"%s",temps);
            }else if(0.6<=setnum && setnum<0.8){ //add line to validation set
                fprintf(validp,"%s",temps);
            }else{ //add line to test set
                fprintf(testp,"%s",temps);
            }
            free(temps);
            temps = malloc(300*sizeof(char));
            randomizer = (rand()%2);
        }
    }
    
    ////////////////////////////////////
    
    if(fclose(fp1)!= 0){
        fprintf(stderr, "Couldn't close poscliques.csv file.\n");
    }
    if(fclose(fp2)!= 0){
        fprintf(stderr, "Couldn't close negcliques.csv file.\n");
    }
    if(fclose(fp3)!= 0){
        fprintf(stderr, "Couldn't close randresults.csv file.\n");
    }
    if(fclose(trainp)!= 0){
        fprintf(stderr, "Couldn't close TrainingSet.csv file.\n");
    }
    if(fclose(validp)!= 0){
        fprintf(stderr, "Couldn't close ValidationSet.csv file.\n");
    }
    if(fclose(testp)!= 0){
        fprintf(stderr, "Couldn't close TestSet.csv file.\n");
    }
}

void print_clique(int opt, list_node *root, FILE* fp){
    list_node *temp = root;
    list_node *temp_2;
    while(temp != NULL){
        temp_2 = temp->next_in_clique;
        while(temp_2 != NULL){
            fprintf(fp,"%s, %s, 1\n", temp->id, temp_2->id);
            temp_2 = temp_2->next_in_clique;
        }
        temp = temp->next_in_clique;
    }
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