//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <dirent.h>
#include "../parse.h"
#include "../tuplist.h"
#include "../datatypes.h"
#include "acutest.h"
#define BUFFERCAP 100

#define PATH "camera_specs/2013_camera_specs/" // WE SHALL ALLOW THE USER TO ENTER THE PATH THROUGH 
// KEYBOARD INPUT, BUT LATER 

void test_JSONparse(void){    
    struct dirent *current_folder, *current_file;
    char *path, *file_path = NULL;
    
    //Buffers and buffer counters//
    // buff_name = malloc(BUFFERCAP*sizeof(char)); // atr name buffer
    // buff_val = malloc(BUFFERCAP*sizeof(char));  // atr val buffer
    int reallcount = 1, bufflimit = BUFFERCAP-1;
    //////////////////////////////
    
    //Tuple list//
    tuplelist *tulist = NULL;
    int error;
    /////////////
    
    
    path = malloc((strlen(PATH) + 1)*sizeof(char));
    strcpy(path, PATH);
    printf("About to traverse folder %s\n", path); //DEBUGGING INFO
    
    DIR *dir = opendir(path);
    DIR *child_dir;
    FILE *fp;
    //FILE *jfp; //file pointer of json file contents
    int curr_fpl, curr_path_length = strlen(PATH) + 1; //curr_fpl is path to current file rather than
    // curr_path_length which is path to current folder
    int c, atrpass = 0,ecount = 2; //will be used in reading json files, ecount = " counter, atr_pass for switching buffers (0 for name, 1 for val)
    int files_trav = 0;
    size_t line_size = 1024;
    int bytes_read;
    char *line, *buff_name, *buff_val;
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
                    // if((strcmp(current_folder->d_name, "cammarkt.com") == 0) && (strcmp(current_file->d_name, "0.json") == 0)){
                        // if(strcmp(current_folder->d_name, "cammarkt.com") != 0){
                            if((strcmp(current_file->d_name, ".") != 0) && (strcmp(current_file->d_name, "..") != 0)){ 
                                printf("Going in %s %s file\n", current_folder->d_name, current_file->d_name);
                                fp = fopen(file_path,"r");
                                if(fp == NULL){
                                    fprintf(stderr, "Couldn't access JSON file.\n");
                                }
                                list_create(&tulist, &error); //initializing tuplelist
                                if(error==1){
                                    fprintf(stderr, "Failed to create tuplelist.\n");
                                }
                                
                                line = malloc(line_size*sizeof(char));
                                bytes_read = getline(&line, &line_size, fp);
                                // free(line);
                                buff_name = malloc(line_size*sizeof(char));
                                buff_val = malloc(line_size*sizeof(char));
                                
                                while(1){
                                    bytes_read = getdelim(&buff_name, &line_size, ':', fp);
                                    if(strcmp(buff_name, "}") == 0){
                                        break;
                                    }
                                    buff_name = strtok(buff_name, ":");
                                    getline(&buff_val, &line_size, fp);
                                    // printf("buff_val: %s\n", buff_val);
                                    if(strcmp(buff_val, " [\n") == 0){ // JSON Array
                                        // printf("GETS IN HERE\n");
                                        buff_val = strtok(buff_val, "\n");
                                        while(1){
                                            bytes_read = getline(&line, &line_size, fp);
                                            // buff_val = realloc(buff_val, (strlen(buff_val) + bytes_read + 1 )*sizeof(char));
                                            // strcat(buff_val, line);
                                            line = strtok(line, "\n");
                                            // printf("strtok is ok\n");
                                            // if(strcmp(line, "],") == 0){
                                            if(strstr(line, "]")){
                                                // end of array
                                                // printf("end of array\n");
                                                // strtok(line, ",");
                                                // strcat(buff_val, "]");
                                                // printf("array buff_val = %s\n", buff_val);
                                                break;
                                            }
                                            // strcat(buff_val, line);
                                            strcpy(buff_val, "array");
                                        }
                                        
                                    }else{
                                        // buff_val = strtok(buff_val, ",");
                                    }
                                    
                                    list_insert(&tulist, buff_name, buff_val);
                                }
                                node_tuple *temp = tulist->head;
                                while(temp){
                                    // printf("%s %s\n", temp->name, temp->value);
                                    temp = temp->next;
                                }
                                
                                if(fclose(fp) != 0){
                                    fprintf(stderr, "Couldn't close JSON file.\n");
                                }
                                list_destroy(&tulist, &error);
                            }
                        // }
                    // }
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
}

TEST_LIST = {
    {"testjson", test_JSONparse},
    {NULL, NULL}
};