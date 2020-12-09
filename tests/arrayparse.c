#include "acutest.h"
#include "../hashmap.h"
#include "../tuplist.h"
#include "../datatypes.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define FILE_NAME "camera_specs/2013_camera_specs/cammarkt.com/0.json"
#define PATH "camera_specs/2013_camera_specs/"

void test_arrayparsing(void){
    FILE *fp = fopen(FILE_NAME, "r");
    TEST_ASSERT(fp != NULL);

    size_t bytes_read;
    size_t line_size = 1024;
    char *line, *buff_name, *buff_val;
    
    // NEW STUFF //
    char *temp, *temp2;
    int name_capacity = 100;
    int val_capacity = 100;
    int remaining = val_capacity - 1;
    ///////////////
    
    line = malloc(line_size*sizeof(char));
    buff_name = malloc(name_capacity*sizeof(char));
    buff_val = malloc(val_capacity*sizeof(char));
    
    while(1){
        bytes_read = getdelim(&buff_name, &line_size, ':', fp);
        if(strcmp(buff_name, "}") == 0){
            break;
        }
        buff_name = strtok(buff_name, ":");
        getline(&line, &line_size, fp);
        if(strcmp(line, " [\n") == 0){ // JSON Array
            line = strtok(line, "\n");
            while(1){
                bytes_read = getline(&line, &line_size, fp);
                // printf("bytes_read = %ld, val_capacity = %d\n", bytes_read, val_capacity);
                // printf("buff_val = %s\n", buff_val);
                temp = strtok(line, "\n");
                // printf("line = %s\n", line);
                bytes_read--;
                if(strstr(temp, "]")){
                    // end of array
                    // strtok(line, ",");
                    // strcat(buff_val, "]");
                    printf("%s\n", buff_val);
                    printf("\n\n\n");
                    memset(buff_val, 0, val_capacity);
                    remaining = val_capacity - 1;
                    break;
                }
                if((strcmp(buff_val, " [") == 0) && (bytes_read < remaining)){
                    strncpy(buff_val, temp, remaining);
                }else if((strcmp(buff_val, " [") == 0) && (bytes_read >= remaining)){
                    while(bytes_read >= remaining){
                        val_capacity *= 2;
                        remaining += val_capacity/2;
                    }
                    buff_val = realloc(buff_val, val_capacity*sizeof(char));
                    strncpy(buff_val, temp, remaining);
                }else if((strcmp(buff_val, " [") != 0) && (bytes_read < remaining)){
                    strncat(buff_val, temp, remaining);
                }else{
                    while(bytes_read >= remaining){
                        val_capacity *= 2;
                        remaining += val_capacity/2;
                    }
                    // printf("val_capacity is now %d and remaining is %d\n", val_capacity, remaining);
                    // printf("right before realloc, length is %ld and buff is %s", strlen(buff_val), buff_val);
                    temp2 = realloc(buff_val, val_capacity*sizeof(char));
                    TEST_ASSERT(temp2 != NULL);
                    buff_val = temp2;
                    strncat(buff_val, temp, remaining);
                }
                remaining = remaining - bytes_read;
                // strcpy(buff_val, "array");
            }
        }
        // tuplist_insert(&tulist, buff_name, buff_val);
    }
    free(line);
    free(buff_name);
    free(buff_val);
    TEST_ASSERT(fclose(fp) == 0);
} 

void test_complete_arrayparsing(void){
    struct dirent *current_folder, *current_file;
    char *path, *file_path = NULL;
    char *id_buf; //Will be the buffer for the node's creation and will hold their id
    
    //Map//
    hash_map *map = create_map();
    
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

    // NEW //
    curr_fpl = curr_path_length + 20; 
    // a few extra characters to make sure site and file name fit
    // at least for the first directory
    file_path = malloc(curr_fpl*sizeof(char));
    char *json_postfix;
    char *temp_realloc;
    int id_buf_len = 100;
    id_buf = malloc(id_buf_len*sizeof(char));
    /////////
    
    
    size_t line_size = 1024;
    int bytes_read;
    char *line, *buff_name, *buff_val;
    
    //NEW//
    line = malloc(line_size*sizeof(char));
    size_t buff_name_size = 1024;
    size_t buff_val_size = 1024;
    buff_name = malloc(buff_name_size*sizeof(char));
    buff_val = malloc(buff_val_size*sizeof(char));
    int val_capacity = 100;
    int remaining = val_capacity - 1;
    char *array_buff = malloc(val_capacity*sizeof(char));
    ///////

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
        if(curr_path_length < strlen(PATH) + strlen(current_folder->d_name) + 1){
            curr_path_length = strlen(PATH) + strlen(current_folder->d_name) + 1;
            temp_realloc = realloc(path, curr_path_length*sizeof(char));
            TEST_ASSERT(temp_realloc != NULL);
            path = temp_realloc;
        }
        strcpy(path, PATH);
        strcat(path, current_folder->d_name);
        
        
        child_dir = opendir(path); // reading site sub-dir contents (json files)
        
        
        while((current_file = readdir(child_dir)) != NULL){
            if(curr_fpl < curr_path_length + strlen(current_file->d_name) + 2){
                curr_fpl = curr_path_length + strlen(current_file->d_name) + 2;
                temp_realloc = realloc(file_path, curr_fpl*sizeof(char));
                TEST_ASSERT(temp_realloc != NULL);
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
                    TEST_ASSERT(temp_realloc != NULL);
                    id_buf = temp_realloc;
                }
                strcpy(id_buf, current_folder->d_name);
                strcat(id_buf, "//");
                strncat(id_buf, current_file->d_name, sans_json_length);
                
                int hash_val = hash_function(map, id_buf);

                // NODE CREATION
                list_node* pseudonode = create_node(id_buf);

                fp = fopen(file_path,"r");
                TEST_ASSERT(fp != NULL);
                tuplist_create(&tulist, &error); //initializing tuplelist
                TEST_ASSERT(error!=1);
                
                
                bytes_read = getline(&line, &line_size, fp);
                
                while(1){
                    bytes_read = getdelim(&buff_name, &buff_name_size, ':', fp);
                    if(strcmp(buff_name, "}") == 0){
                        break;
                    }
                    buff_name = strtok(buff_name, ":");
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
                                TEST_ASSERT(temp_realloc != NULL);
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
                                TEST_ASSERT(temp_realloc != NULL);
                                array_buff = temp_realloc;
                                strncat(array_buff, line, remaining);
                            }
                            remaining = remaining - bytes_read;
                        }
                        tuplist_insert(&tulist, buff_name, array_buff);
                    }else{
                        tuplist_insert(&tulist, buff_name, buff_val);
                    }
                }
                
                if(fclose(fp) != 0){
                    fprintf(stderr, "Couldn't close JSON file.\n");
                }
                
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
    
    list_node *tempn = find_node(map, "www.ebay.com//44865");
    printf("\n\n\n");
    tuplist_print(&(tempn->content));
    destroy_map(&map);
}

TEST_LIST = {
    {"arrays", test_arrayparsing},
    {"whole_dataset_arrays", test_complete_arrayparsing},
    {NULL, NULL}
};
