#include "acutest.h"
#include <dirent.h>
#include "../BOW/bow.h"
#include "../BOW/stringOps.h"
#include "../BOW/vectorOps.h"
#include "../BOW/stopwords.h"
#include "../TF-IDF/idfVectorOps.h"

#define PATH "tests/bow_test_set/"

void test_all_bow_strctures(void){
    struct dirent *current_folder, *current_file;
    char *path, *file_path = NULL;
    
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
    char *temp_realloc;

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
    
    sw_list *l;
    make_stopword_list(&l);
    
    tree_node *tree;
    create_tree(&tree);
    
    BoW *bag;
    create_bow(&bag);
    TEST_ASSERT(bag != NULL);
    
    TEST_ASSERT(dir != NULL);
    current_folder = readdir(dir);
    TEST_ASSERT(current_folder != NULL);
    
    int text_counter = 0;
    
    IDFVector *idf_vec;
    create_idf_vector(&idf_vec);
    
    char *json_postfix;
    int id_buf_len = 100;
    char *id_buf = malloc(id_buf_len*sizeof(char));
    
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
                
                fp = fopen(file_path,"r");
                text_counter++;
                TEST_ASSERT(fp != NULL);
                
                new_text_file(bag, text_counter, id_buf);
                
                bytes_read = getline(&line, &line_size, fp);
                
                while(1){
                    bytes_read = getdelim(&buff_name, &buff_name_size, ':', fp);
                    if(strcmp(buff_name, "}") == 0){
                        break;
                    }
                    buff_name = strtok(buff_name, ":");
                    bow_it(buff_name, l, &tree, &bag, text_counter, idf_vec);
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
                        bow_it(array_buff, l, &tree, &bag, text_counter, idf_vec);
                    }else{
                        bow_it(buff_val, l, &tree, &bag, text_counter, idf_vec);
                    }
                }
                TEST_ASSERT(fclose(fp) == 0);
            }
        }
        closedir(child_dir);
        current_folder = readdir(dir);
    }
    // for(int i = 1; i < VECTORS; i++){
        // printf("VECTOR %d.\n", i);
        // for(int j = 0; j < bag->vectors[i]->size; j++){
            // printf("word: %d frequency: %d\n", bag->vectors[0]->elements[j], bag->vectors[i]->elements[j]);            
        // }
        // printf("\n");
    // }
    // printf("IDF VECTOR\n");
    // for(int i = 0; i < idf_vec->size; i++){
        // if(i == 0 || i == 1){
            // TEST_ASSERT(idf_vec->elements[i] == (double) 5);
        // }
        // printf("word %d appears in %d out of 5 texts\n", i, (int) idf_vec->elements[i]);
    // }
    compute_idf_vals(idf_vec);
    // for(int i = 0; i < idf_vec->size; i++){
        // if(i == 0 || i == 1){
            // TEST_ASSERT(idf_vec->elements[i] == (double) 0);
        // }
        // printf("word %d has idf value of %lf\n", i, idf_vec->elements[i]);
    // }
    for(int i = 1; i < VECTORS; i++){
        printf("File %s contains %d words\n", bag->vectors[i]->name, bag->vectors[i]->word_count);
    }
    // print_tree(tree);
    destroy_tree(&tree);
    TEST_ASSERT(tree == NULL);
    free(path);
    free(line);
    free(buff_name);
    free(buff_val);
    closedir(dir);
    destroy(&l);
    TEST_ASSERT(l == NULL);
    destroy_bow(&bag);
    TEST_ASSERT(bag == NULL);
    destroy_idf_vector(&idf_vec);
    TEST_ASSERT(idf_vec == NULL);
}    

TEST_LIST = {
    {"all", test_all_bow_strctures},
    {NULL, NULL}
};