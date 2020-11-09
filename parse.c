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
    FILE *jfp; //file pointer of json file contents
    int curr_fpl, curr_path_length = strlen(PATH) + 1; //curr_fpl is path to current file rather than
    // curr_path_length which is path to current folder
    int c, ecount = 2; //will be used in reading json files, ecount = " counter
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
                if((strcmp(current_folder->d_name, "buy.net") == 0)&&(strcmp(current_file->d_name, "4236.json") == 0)){
                    fp = fopen(file_path,"r");
                    if(fp == NULL){
                        fprintf(stderr, "Couldn't access JSON file.\n");
                    }
                    // will use fgetc, and for starters try to print contents into a text file //
                    jfp = fopen("jsontest.txt","w");
                    do{
                        c = fgetc(fp);
                        if(feof(fp)){
                            break;
                        }
                        if(c == ':'){ //About to start parsing attribute value
                            ecount = 2;
                            fputc('\t',jfp);
                        }
                        if(c == ',' && ecount ==0){ //Line parsing completed, moving to next attribute
                            ecount = 2;
                            fputc('\n', jfp);
                        }
                        //character control | Attribute/Value parsing and writing in test file//
                        if((c=='"') && (ecount == 2)){
                            ecount--;
                            while(ecount>0){
                                c = fgetc(fp);
                                if(feof(fp)){
                                    break;
                                }
                                if(c!='"'){//keeping attribute name and value information
                                    if(c == 92){//some entries have \" in them which messes with value output
                                        fputc(c, jfp);
                                        c = fgetc(fp);
                                        if(c== '"'){
                                            fputc(c,jfp);
                                            continue;
                                        }
                                        
                                    }
                                   fputc(c, jfp); 
                                }else{
                                    ecount--;
                                }
                            }                        
                        }
                        //////////////////////
                    }while(1);
                    //printf("Out of loop\n");
                    if(fclose(jfp)!= 0){
                        fprintf(stderr, "Couldn't close JSON file.\n");
                    }
                    ////////////////////////////////////////////////////////////////////////////
                    if(fclose(fp)!= 0){
                        fprintf(stderr, "Couldn't close file.\n");
                    }
                }
                //Preparation before switching files
                ecount = 2;// "reinitializing ecount to move on the next file
                    // this helps in avoiding to skip the first attribute name (value is being registered) when switching files
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
