#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <assert.h>
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

void parser(hash_map* map, sw_list *l, BoW *bag, tree_node **dict, Vector *idf_vec){
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
    int curr_fpl, curr_path_length = strlen(PATH) + 1; 
    // curr_path_length doesn't contain the actual size of the path
    // but rather the size of the path buffer
    // curr_fpl does the same thing for the *file* path buffer

    curr_fpl = curr_path_length + 20; 
    // a few extra characters to make sure site and file name fit
    // at least for the first directory
    file_path = malloc(curr_fpl*sizeof(char));
    char *json_postfix;
    char *temp_realloc;
    int id_buf_len = 100;
    id_buf = malloc(id_buf_len*sizeof(char));
    
    
    size_t line_size = 1024;
    int bytes_read;
    char *line, *buff_name, *buff_val;
    
    line = malloc(line_size*sizeof(char));
    size_t buff_name_size = 1024;
    size_t buff_val_size = 1024;
    buff_name = malloc(buff_name_size*sizeof(char));
    buff_val = malloc(buff_val_size*sizeof(char));
    int val_capacity = 100;
    int remaining = val_capacity - 1;
    char *array_buff = malloc(val_capacity*sizeof(char));

    assert(dir != NULL);
    current_folder = readdir(dir);
    assert(current_folder != NULL);
    int text_counter = 0;
    
    if(strcmp(current_folder->d_name, ".") == 0){ // Bypassing cases of dot... 
        current_folder = readdir(dir);
        if(strcmp(current_folder->d_name, "..") == 0){ // ...and dot-dot entries existing
            current_folder = readdir(dir); // reading site sub-directories
        }
    }
    
    while(current_folder != NULL){
        if(curr_path_length < strlen(PATH) + strlen(current_folder->d_name) + 1){
            curr_path_length = strlen(PATH) + strlen(current_folder->d_name) + 1;
            temp_realloc = realloc(path, curr_path_length*sizeof(char));
            assert(temp_realloc != NULL);
            path = temp_realloc;
        }
        strcpy(path, PATH);
        strcat(path, current_folder->d_name);
        
        
        child_dir = opendir(path); // reading site sub-dir contents (json files)
        
        
        while((current_file = readdir(child_dir)) != NULL){
            if(curr_fpl < curr_path_length + strlen(current_file->d_name) + 2){
                curr_fpl = curr_path_length + strlen(current_file->d_name) + 2;
                temp_realloc = realloc(file_path, curr_fpl*sizeof(char));
                assert(temp_realloc != NULL);
                file_path = temp_realloc;
            }

            strcpy(file_path, path);
            strcat(file_path, "/");
            strcat(file_path, current_file->d_name);

            if((strcmp(current_file->d_name, ".") != 0) && (strcmp(current_file->d_name, "..") != 0)){ 
                
                // NODE ID CREATION
                json_postfix = strchr(current_file->d_name, '.');
                int sans_json_length = strlen(current_file->d_name) - strlen(json_postfix);
                if(id_buf_len < strlen(current_folder->d_name) + sans_json_length +3 ){
                    id_buf_len *= 2;
                    temp_realloc = realloc(id_buf, id_buf_len*sizeof(char));
                    assert(temp_realloc != NULL);
                    id_buf = temp_realloc;
                }
                strcpy(id_buf, current_folder->d_name);
                strcat(id_buf, "//");
                strncat(id_buf, current_file->d_name, sans_json_length);
                
                int hash_val = hash_function(map, id_buf);

                // NODE CREATION
                list_node* pseudonode = create_node(id_buf);

                fp = fopen(file_path,"r");
                assert(fp != NULL);
                text_counter++;
                tuplist_create(&tulist, &error); //initializing tuplelist
                assert(error!=1);
                
                
                bytes_read = getline(&line, &line_size, fp);
                
                while(1){
                    bytes_read = getdelim(&buff_name, &buff_name_size, ':', fp);
                    if(strcmp(buff_name, "}") == 0){
                        break;
                    }
                    buff_name = strtok(buff_name, ":");
                    bow_it(buff_name, l, dict, &bag, text_counter, idf_vec);
                    getline(&buff_val, &buff_val_size, fp);
                    if(strcmp(buff_val, " [\n") == 0){ // JSON Array
                        strcpy(array_buff, " [");
                        while(1){
                            bytes_read = getline(&line, &line_size, fp);
                            line = strtok(line, "\n");
                            bytes_read--;
                            if(strstr(line, "]")){
                                remaining = val_capacity - 1;
                                break;
                            }
                            if((strcmp(array_buff, " [") == 0) && (bytes_read < remaining)){
                                strncpy(array_buff, line, remaining);
                            }else if((strcmp(array_buff, " [") == 0) && (bytes_read >= remaining)){
                                while(bytes_read >= remaining){
                                    val_capacity *= 2;
                                    remaining += val_capacity/2;
                                }
                                temp_realloc = realloc(array_buff, val_capacity*sizeof(char));
                                assert(temp_realloc != NULL);
                                array_buff = temp_realloc;
                                strncpy(array_buff, line, remaining);
                            }else if((strcmp(array_buff, " [") != 0) && (bytes_read < remaining)){
                                strncat(array_buff, line, remaining);
                            }else{
                                while(bytes_read >= remaining){
                                    val_capacity *= 2;
                                    remaining += val_capacity/2;
                                }
                                temp_realloc = realloc(array_buff, val_capacity*sizeof(char));
                                assert(temp_realloc != NULL);
                                array_buff = temp_realloc;
                                strncat(array_buff, line, remaining);
                            }
                            remaining = remaining - bytes_read;
                        }
                        bow_it(array_buff, l, dict, &bag, text_counter, idf_vec);
                        tuplist_insert(&tulist, buff_name, array_buff);
                    }else{
                        bow_it(buff_val, l, dict, &bag, text_counter, idf_vec);
                        tuplist_insert(&tulist, buff_name, buff_val);
                    }
                }
                assert(fclose(fp) == 0);
                pseudonode->content = tulist;
                tulist = NULL;
                add_to_bucket(map, hash_val, pseudonode);
            }
        }
        closedir(child_dir);
        current_folder = readdir(dir);
    }

    free(path);
    free(id_buf);
    free(line);
    free(buff_name);
    free(buff_val);
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
