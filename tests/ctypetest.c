#include "acutest.h"
#include "../tuplist.h"
#include "../BOW/bow.h"
#include <ctype.h>

#define FILE_NAME "camera_specs/2013_camera_specs/cammarkt.com/83.json"

void test_attribute_value_buffer_splitting(){
    //Tuple list//
    tuplelist *tulist = NULL;
    int error;
    /////////////
    
    FILE *fp;
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
    
    fp = fopen(FILE_NAME,"r");
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
        bow_it(buff_name);
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
            bow_it(array_buff);
            tuplist_insert(&tulist, buff_name, array_buff);
        }else{
            bow_it(buff_val);
            tuplist_insert(&tulist, buff_name, buff_val);
        }
    }
    
    TEST_ASSERT(fclose(fp) == 0);
    free(buff_name);
    free(buff_val);
    free(line);
    free(array_buff);
    tuplist_destroy(&tulist, &error);
}

TEST_LIST = {
    {"string splitting", test_attribute_value_buffer_splitting},
    {NULL, NULL}
};