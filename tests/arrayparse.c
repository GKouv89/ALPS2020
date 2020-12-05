#include "acutest.h"
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "camera_specs/2013_camera_specs/cammarkt.com/0.json"

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

TEST_LIST = {
    {"arrays", test_arrayparsing},
    {NULL, NULL}
};
