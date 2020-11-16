#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "parse.h"

#include "datatypes.h"
#include "hashmap.h"
#include "tuplist.h"
#include "set.h"

#define PATH "camera_specs/2013_camera_specs/" // WE SHALL ALLOW THE USER TO ENTER THE PATH THROUGH 
// KEYBOARD INPUT, BUT LATER 

#ifndef DATASET
    #define DATASET "sigmod_large_labelled_dataset.csv"
#endif

void parser(hash_map* map){
    struct dirent *current_folder, *current_file;
    char *path, *file_path = NULL;
    char *id_buf; //Will be the buffer for the node's creation and will hold their id
    
    //Tuple list//
    tuplelist *tulist = NULL;
    int error;
    /////////////
    
    path = malloc((strlen(PATH) + 1)*sizeof(char));
    strcpy(path, PATH);
   
    DIR *dir = opendir(path);
    DIR *child_dir;
    FILE *fp;
    int curr_fpl, curr_path_length = strlen(PATH) + 1; //curr_fpl is path to current file rather than
    // curr_path_length which is path to current folder

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
        }
    }
    
    while(current_folder != NULL){
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

            if((strcmp(current_file->d_name, ".") != 0) && (strcmp(current_file->d_name, "..") != 0)){ 
                
                // NODE ID CREATION
                char* temp;
                temp = strchr(current_file->d_name, '.');
                int length = strlen(current_file->d_name) - strlen(temp);
                id_buf = malloc((strlen(current_folder->d_name) + strlen(current_file->d_name) + 3)*sizeof(char));
                strcpy(id_buf, current_folder->d_name);
                strcat(id_buf, "//");
                strncat(id_buf, current_file->d_name, length);
                int hash_val = hash_function(map, id_buf);

                // NODE CREATION
                list_node* pseudonode = create_node(id_buf);

                fp = fopen(file_path,"r");
                if(fp == NULL){
                    fprintf(stderr, "Couldn't access JSON file.\n");
                }
                tuplist_create(&tulist, &error); //initializing tuplelist
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
                    if(strcmp(buff_val, " [\n") == 0){ // JSON Array
                        buff_val = strtok(buff_val, "\n");
                        while(1){
                            bytes_read = getline(&line, &line_size, fp);
                            // buff_val = realloc(buff_val, (strlen(buff_val) + bytes_read + 1 )*sizeof(char));
                            // strcat(buff_val, line);
                            line = strtok(line, "\n");
                            if(strstr(line, "]")){
                                // end of array
                                // strtok(line, ",");
                                // strcat(buff_val, "]");
                                break;
                            }
                            strcpy(buff_val, "array");
                        }
                        
                    }else{
                        // buff_val = strtok(buff_val, ",");
                    }
                    
                    tuplist_insert(&tulist, buff_name, buff_val);
                }
                
                if(fclose(fp) != 0){
                    fprintf(stderr, "Couldn't close JSON file.\n");
                }
                
                pseudonode->content = tulist;
		tulist = NULL;
                add_to_bucket(map, hash_val, pseudonode);
                free(id_buf);      
            }
        }
        closedir(child_dir);
        current_folder = readdir(dir);
    }
    free(path);
    closedir(dir);
}

void csvparser(hash_map* map, clique_list* all_cliques){
    
    FILE *fp;
    char *site_buff1;
    char *site_buff2;
    char matching;
    int c, chars_written = 0;
    int flag = 0; // used in eof checking
    
    //buffer memory alloc, in no case are more than 100 chars written for the csv fields//
    site_buff1 = malloc(100*sizeof(char));
    site_buff2 = malloc(100*sizeof(char)); 
    
    fp = fopen(DATASET,"r");

    if(fp == NULL){
        fprintf(stderr, "Couldn't open csv file.\n");
    }
    // first csv line doesn't contain useful info so
    // it'll be ignored
    do{
        c = fgetc(fp);
    }while(c != '\n'); // c should be \n
    
    while(1){
        while((c = fgetc(fp))!=','){
            // if EOF is read, it will be done here, instead of reading the first 
            // column of a new line.
            if(feof(fp)){
                flag = 1;
                break;
            }
            chars_written++;
            if(chars_written == 1){
                strcpy(site_buff1, (char*)&c);
            }else{
                strcat(site_buff1, (char*)&c);
            }
        }
        if(flag){ // If flag==1 then we've reached 
                  //the last (blank) line in the file and EOF
            break;
        }
        chars_written = 0;
        
        while((c = fgetc(fp))!=','){ //no need to check for EOF here
            chars_written++;
            if(chars_written == 1){
                strcpy(site_buff2, (char*)&c);
            }else{
                strcat(site_buff2, (char*)&c);
            }
        }
        
        matching = fgetc(fp);
        if(matching == '1'){
            list_node *a, *b;
            a = find_node(map, site_buff1);
            b = find_node(map, site_buff2);
            join_sets(all_cliques, a, b);
        }
        c = fgetc(fp); // removing \n from being added at the
                       // start of next buffer
        free(site_buff1);
        site_buff1 = malloc(100*sizeof(char));
        free(site_buff2);
        site_buff2 = malloc(100*sizeof(char));
    } 
    if(fclose(fp)!= 0){
        fprintf(stderr, "Couldn't close csv file.\n");
    }
    
}
