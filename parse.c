#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#define PATH "camera_specs/2013_camera_specs/"
#define BUY_NET "buy.net"

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