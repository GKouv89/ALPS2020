#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "parse.h"

#define PATH "camera_specs/2013_camera_specs/" // WE SHALL ALLOW THE USER TO ENTER THE PATH THROUGH 
// KEYBOARD INPUT, BUT LATER 

void parser(){
    struct dirent *current_folder, *current_file;
    char *path, *file_path = NULL;
    path = malloc((strlen(PATH) + 1)*sizeof(char));
    strcpy(path, PATH);
    printf("About to traverse folder %s\n", path); //DEBUGGING INFO
    DIR *dir = opendir(path);
    DIR *child_dir;
    FILE *fp;
    int curr_fpl, curr_path_length = strlen(PATH) + 1; //curr_fpl is path to current file rather than
    // curr_path_length which is path to current folder
    if(dir == NULL){
        fprintf(stderr, "sth went wrong\n");
        return;
    }
    current_folder = readdir(dir);
	if(current_folder == NULL){
        fprintf(stderr, "could not access 2013_camera_specs contents.\n");
    }
    if(strcmp(current_folder->d_name, ".") == 0){ // Bypassing cases of dot... 
        current_folder = readdir(dir);
        if(strcmp(current_folder->d_name, "..") == 0){ // ...and dot-dot entries existing
            current_folder = readdir(dir); // reading site sub-directories
			while(current_folder != NULL){
				//printf("Current folder: %s\n", current_folder->d_name);
                memset(path, 0, curr_path_length*sizeof(char)); // resetting path name
                curr_path_length = strlen(PATH) + strlen(current_folder->d_name) + 1;
                path = realloc(path, curr_path_length*sizeof(char));
                strcpy(path, PATH);
                strcat(path, current_folder->d_name);
                child_dir = opendir(path); // reading site sub-dir contents (json files)
                while((current_file = readdir(child_dir)) != NULL){
                    if(file_path != NULL){
                        memset(file_path, 0, curr_fpl*sizeof(char)); // resetting file name   
                    }
                    curr_fpl = strlen(path) + strlen(current_file->d_name) + 2*sizeof(char);
                    file_path = realloc(file_path, curr_fpl*sizeof(char));
                    strcpy(file_path, path);
                    strcat(file_path, "/");
                    strcat(file_path, current_file->d_name);
                    //printf("File_path: %s\n", file_path);
				//
				//
				//
					fp = fopen(file_path,"r");
					if(fp == NULL){
						fprintf(stderr, "Couldn't access JSON file.\n");
					}
					//will use fgetc
					if(fclose(fp)!= 0){
						fprintf(stderr, "Couldn't close file.\n");
					}
					
				//
				//
				//
                    
                }
                closedir(child_dir);
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
    free(path);
    closedir(dir);
}
