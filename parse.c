#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "parse.h"

#define PATH "camera_specs/2013_camera_specs/" // WE SHALL ALLOW THE USER TO ENTER THE PATH THROUGH 
// KEYBOARD INPUT, BUT LATER 

void parser(){
    struct dirent *current_folder;
    char *path;
    path = malloc((strlen(PATH) + 1)*sizeof(char));
    strcpy(path, PATH);
    path[strlen(path)-1] = '\0';
    // printf("About to traverse folder %s\n", path); //DEBUGGING INFO
    DIR *dir = opendir(path);
    if(dir == NULL){
        fprintf(stderr, "sth went wrong\n");
        return;
    }
    current_folder = readdir(dir);
	if(current_folder == NULL){
        fprintf(stderr, "could not access 2013_camera_specs contents.\n");
    }
    if(strcmp(current_folder->d_name, ".") == 0){
        current_folder = readdir(dir);
        if(strcmp(current_folder->d_name, "..") == 0){
            current_folder = readdir(dir);
			while(current_folder != NULL){
				printf("Current folder: %s\n", current_folder->d_name);
				current_folder = readdir(dir);
			}
        }else{
			while(current_folder != NULL){
				printf("Current folder: %s\n", current_folder->d_name);
				current_folder = readdir(dir);
			}
        }
    }else{
		while(current_folder != NULL){
			printf("Current folder: %s\n", current_folder->d_name);
			current_folder = readdir(dir);
		}
	}
    closedir(dir);
}



/*
int parsing_dataset(){
    if(parse_buy_net() == -1){
        return -1;
    } 
    return 0;
}

int parse_buy_net(){
    int lower_bound = 4233;
    // int upper_bound = 6785;
    char *file_name, *spec_id;
    int name_length = strlen(PATH);
    name_length += strlen(BUY_NET);
    name_length += 10;
    file_name = malloc(name_length*sizeof(char));
    spec_id = malloc(5*sizeof(char));
    strcpy(file_name, PATH);
    strcat(file_name, BUY_NET);
    sprintf(spec_id, "%d", lower_bound);
    strcat(file_name, spec_id);
    strcat(file_name, ".json");
    FILE *spec = fopen(file_name, "r");
    if(spec == NULL){
        free(file_name);
        free(spec_id);
        return -1;
    }
    if(fclose(spec) != 0){
        free(file_name);
        free(spec_id);
        return -1;
    }
    free(file_name);
    free(spec_id);
    return 0;
}
*/