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

void test_firstlineparse(void){
    
    FILE *fp;
    char *site_buff1;
    char *site_buff2;
    char matching;
    int c, reallcount =1, bufflimit = BUFFERCAP-1;
    
    //buffer memory alloc//
    site_buff1 = malloc(BUFFERCAP*sizeof(char));
    site_buff2 = malloc(BUFFERCAP*sizeof(char)); // may need to realloc
    
    fp = fopen("sigmod_medium_labelled_dataset.csv","r");
    TEST_ASSERT(fp != NULL); // forced program close on error
    //TEST_MSG("Couldn't open csv file.\n")
    // first csv line doesn't contain useful info so
    // it'll be ignored
    do{
        c = fgetc(fp);
    }while(c != '\n'); // c should be \n
    while((c = fgetc(fp))!=','){
            strcat(site_buff1, (char*)&c);
            bufflimit--;
            if(bufflimit == 0){// low chance of this happening
                               // Boundary testing in case urls were larger
                reallcount++;
                site_buff1 = realloc(site_buff1, reallcount*BUFFERCAP);
            }
    }
    
    TEST_CHECK(strcmp(site_buff1, "www.garricks.com.au//31") == 0);
    TEST_MSG("Site_buff1 content: %s\n", site_buff1);
    
    free(site_buff1);
    site_buff1 = malloc(BUFFERCAP*sizeof(char));
    bufflimit = BUFFERCAP-1;
    reallcount = 1; //reinitializing bufflimit and reallcount for next buffer
    
    while((c = fgetc(fp))!=','){
            strcat(site_buff2, (char*)&c);
            bufflimit--;
            if(bufflimit == 0){// low chance of this happening
                               // Boundary testing in case urls were larger
                reallcount++;
                site_buff2 = realloc(site_buff2, reallcount*BUFFERCAP);
            }
        }
        
    TEST_CHECK(strcmp(site_buff2, "www.ebay.com//53278") == 0);
    TEST_MSG("Site_buff2 content: %s\n", site_buff2);   
     
    free(site_buff2);
    site_buff2 = malloc(BUFFERCAP*sizeof(char));
    bufflimit = BUFFERCAP-1;
    reallcount = 1; //reinitializing bufflimit and reallcount for next buffer
    //moving to id
    matching = fgetc(fp);
    c = fgetc(fp); // removing \n from being added at the
                   // start of next buffer  
    TEST_CHECK(c == '\n');
    TEST_MSG("c isn't new line char");
    TEST_CHECK(fclose(fp)== 0);
    TEST_MSG("Couldn't close csv file.\n");  
}

void test_eofparse(void){
    FILE *fp;
    char *site_buff1;
    char *site_buff2;
    char matching;
    int c, reallcount = 1, bufflimit = BUFFERCAP-1;
    int flag = 0;
    
    //buffer memory alloc//
    site_buff1 = malloc(BUFFERCAP*sizeof(char));
    site_buff2 = malloc(BUFFERCAP*sizeof(char)); // may need to realloc
    
    fp = fopen("sigmod_medium_labelled_dataset.csv","r");
    TEST_ASSERT(fp != NULL); // forced program close on error
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
            strcat(site_buff1, (char*)&c);
            bufflimit--;
            if(bufflimit == 0){// low chance of this happening
                               // Boundary testing in case urls were larger
                reallcount++;
                site_buff1 = realloc(site_buff1, reallcount*BUFFERCAP);
            }
        }
        if(flag){ // If flag==1 then we've reached 
                  //the last (blank) line in the file and EOF
            break;
        }
        bufflimit = BUFFERCAP-1;
        reallcount = 1; //reinitializing bufflimit and reallcount for next buffer
        
        while((c = fgetc(fp))!=','){ //no need to check for EOF here
            strcat(site_buff2, (char*)&c);
            bufflimit--;
            if(bufflimit == 0){// low chance of this happening
                               // Boundary testing in case urls were larger
                reallcount++;
                site_buff2 = realloc(site_buff2, reallcount*BUFFERCAP);
            }
        }
        bufflimit = BUFFERCAP-1;
        reallcount = 1; //reinitializing bufflimit and reallcount for next buffer
        
        matching = fgetc(fp);
        c = fgetc(fp); // removing \n from being added at the
                       // start of next buffer
        free(site_buff1);
        site_buff1 = malloc(BUFFERCAP*sizeof(char));
        free(site_buff2);
        site_buff2 = malloc(BUFFERCAP*sizeof(char));
    }
    
    
    TEST_CHECK(fclose(fp) == 0);
    TEST_MSG("Error: Couldn't close csv file.\n");
}

void test_matchinglines_large(void){ 
    FILE *fp;
    char *site_buff1;
    char *site_buff2;
    char matching;
    int c, reallcount = 1, bufflimit = BUFFERCAP-1;
    int flag = 0;
    int matchcount = 0;
    
    //buffer memory alloc//
    site_buff1 = malloc(BUFFERCAP*sizeof(char));
    site_buff2 = malloc(BUFFERCAP*sizeof(char)); // may need to realloc
    
    fp = fopen("sigmod_large_labelled_dataset.csv","r");
    TEST_ASSERT(fp != NULL); // forced program close on error
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
            strcat(site_buff1, (char*)&c);
            bufflimit--;
            if(bufflimit == 0){// low chance of this happening
                               // Boundary testing in case urls were larger
                reallcount++;
                site_buff1 = realloc(site_buff1, reallcount*BUFFERCAP);
            }
        }
        if(flag){ // If flag==1 then we've reached 
                  //the last (blank) line in the file and EOF
            break;
        }
        bufflimit = BUFFERCAP-1;
        reallcount = 1; //reinitializing bufflimit and reallcount for next buffer
        
        while((c = fgetc(fp))!=','){ //no need to check for EOF here
            strcat(site_buff2, (char*)&c);
            bufflimit--;
            if(bufflimit == 0){// low chance of this happening
                               // Boundary testing in case urls were larger
                reallcount++;
                site_buff2 = realloc(site_buff2, reallcount*BUFFERCAP);
            }
        }
        bufflimit = BUFFERCAP-1;
        reallcount = 1; //reinitializing bufflimit and reallcount for next buffer
        
        matching = fgetc(fp);
        if (matching == '1'){
            matchcount++;
        }
        
        c = fgetc(fp); // removing \n from being added at the
                       // start of next buffer
        free(site_buff1);
        site_buff1 = malloc(BUFFERCAP*sizeof(char));
        free(site_buff2);
        site_buff2 = malloc(BUFFERCAP*sizeof(char));
    }
    TEST_CHECK(matchcount == 44039);
    TEST_MSG("Error: Matchcount %d of CSV file doesn't match total of matching sets\n", matchcount);
    
    TEST_CHECK(fclose(fp) == 0);
    TEST_MSG("Error: Couldn't close csv file.\n");
}

void test_matchinglines_medium(void){ 
    FILE *fp;
    char *site_buff1;
    char *site_buff2;
    char matching;
    int c, reallcount = 1, bufflimit = BUFFERCAP-1;
    int flag = 0;
    int matchcount = 0;
    
    //buffer memory alloc//
    site_buff1 = malloc(BUFFERCAP*sizeof(char));
    site_buff2 = malloc(BUFFERCAP*sizeof(char)); // may need to realloc
    
    fp = fopen("sigmod_medium_labelled_dataset.csv","r");
    TEST_ASSERT(fp != NULL); // forced program close on error
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
            strcat(site_buff1, (char*)&c);
            bufflimit--;
            if(bufflimit == 0){// low chance of this happening
                               // Boundary testing in case urls were larger
                reallcount++;
                site_buff1 = realloc(site_buff1, reallcount*BUFFERCAP);
            }
        }
        if(flag){ // If flag==1 then we've reached 
                  //the last (blank) line in the file and EOF
            break;
        }
        bufflimit = BUFFERCAP-1;
        reallcount = 1; //reinitializing bufflimit and reallcount for next buffer
        
        while((c = fgetc(fp))!=','){ //no need to check for EOF here
            strcat(site_buff2, (char*)&c);
            bufflimit--;
            if(bufflimit == 0){// low chance of this happening
                               // Boundary testing in case urls were larger
                reallcount++;
                site_buff2 = realloc(site_buff2, reallcount*BUFFERCAP);
            }
        }
        bufflimit = BUFFERCAP-1;
        reallcount = 1; //reinitializing bufflimit and reallcount for next buffer
        
        matching = fgetc(fp);
        if (matching == '1'){
            matchcount++;
        }
        
        c = fgetc(fp); // removing \n from being added at the
                       // start of next buffer
        free(site_buff1);
        site_buff1 = malloc(BUFFERCAP*sizeof(char));
        free(site_buff2);
        site_buff2 = malloc(BUFFERCAP*sizeof(char));
    }
    TEST_CHECK(matchcount == 3582);
    TEST_MSG("Error: Matchcount %d of CSV file doesn't match total of matching sets\n", matchcount);
    
    TEST_CHECK(fclose(fp) == 0);
    TEST_MSG("Error: Couldn't close csv file.\n");
}

void test_JSONparse(void){    
    struct dirent *current_folder, *current_file;
    char *path, *buff_name, *buff_val, *file_path = NULL;
    
    //Buffers and buffer counters//
    buff_name = malloc(BUFFERCAP*sizeof(char)); // atr name buffer
    buff_val = malloc(BUFFERCAP*sizeof(char));  // atr val buffer
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
                    if((strcmp(current_folder->d_name, "cammarkt.com") == 0) /*&& (strcmp(current_file->d_name, "4239.json") == 0)*/){
                        if((strcmp(current_file->d_name, ".") != 0) && (strcmp(current_file->d_name, "..") != 0)){
                            fp = fopen(file_path,"r");
                            if(fp == NULL){
                                fprintf(stderr, "Couldn't access JSON file.\n");
                            }
                            list_create(&tulist, &error); //initializing tuplelist
                            if(error==1){
                                fprintf(stderr, "Failed to create tuplelist.\n");
                            }
                            // will use fgetc, and for starters try to print contents into a text file //
                            //jfp = fopen("jsontest.txt","w");
                            do{
                                c = fgetc(fp);
                                if(feof(fp)){
                                    break; 
                                }
                                if(c == ':'){ //About to start parsing attribute value
                                    atrpass = 1;
                                    bufflimit = BUFFERCAP-1; //reinitializing bufflimit for second buffer
                                    reallcount = 1;
                                    ecount = 2;
                                    //fputc('\t',jfp);
                                }
                                if((c == ',' && ecount == 0 ) || (c == '\n' && ecount == 0)){ //Line parsing completed, moving to next attribute
                                    list_insert(&tulist, buff_name, buff_val); // passing info to tuple node before
                                                                               // before emptying buffers for next line
                                    bufflimit = BUFFERCAP-1;
                                    reallcount = 1;
                                    // TEST_CHECK((strlen(buff_name)>=1) && (strlen(buff_val)>=1));
                                    // TEST_MSG("Error: 1 or both buffers are empty\n");
                                    free(buff_name);
                                    buff_name = malloc(BUFFERCAP*sizeof(char));
                                    free(buff_val);
                                    buff_val = malloc(BUFFERCAP*sizeof(char));
                                    atrpass = 0; // getting 1st buffer ready to be used
                                    ecount = 2;
                                    //fputc('\n', jfp);
                                }
                                //character control | Attribute/Value parsing and writing in test file//
                                if((c=='"') && (ecount == 2)){
                                    if(atrpass == 0){//using first buffer to insert atr name
                                        ecount--;
                                        while(ecount>0){
                                            c = fgetc(fp);
                                            if(feof(fp)){
                                                break;
                                            }
                                            if(c!='"'){//keeping attribute name and value information
                                                if(c == 92){//some entries have \" in them which messes with value output
                                                    if(strlen(buff_name)==0){
                                                        strcpy(buff_name, (char*)&c);
                                                    }else{
                                                        strcat(buff_name, (char*)&c);
                                                    }
                                                    bufflimit--;
                                                    if(bufflimit == 0){// Boundary testing for really large atr names
                                                        reallcount++;
                                                        bufflimit = BUFFERCAP-1;
                                                        buff_name = realloc(buff_name, reallcount*BUFFERCAP*sizeof(char));
                                                    }
                                                    c = fgetc(fp);
                                                    if(c== '"'){
                                                        if(strlen(buff_name)==0){
                                                            strcpy(buff_name, (char*)&c);
                                                        }else{
                                                            strcat(buff_name, (char*)&c);
                                                        }
                                                        bufflimit--;
                                                        if(bufflimit == 0){
                                                            reallcount++;
                                                            bufflimit = BUFFERCAP-1;
                                                            buff_name = realloc(buff_name, reallcount*BUFFERCAP*sizeof(char));
                                                        }
                                                        continue;
                                                    }
                                                    
                                                }
                                                if(strlen(buff_name)==0){
                                                    strcpy(buff_name, (char*)&c);
                                                }else{
                                                    strcat(buff_name, (char*)&c);
                                                }                                                
                                                bufflimit--;
                                                if(bufflimit == 0){
                                                    reallcount++;
                                                    bufflimit = BUFFERCAP-1;
                                                    buff_name = realloc(buff_name, reallcount*BUFFERCAP);
                                                }
                                            }else{
                                                ecount--;
                                            }
                                        }   
                                    }else{ // have read till ":", switching to buffer 2 for atr val
                                        ecount--;
                                        while(ecount>0){
                                            c = fgetc(fp);
                                            if(feof(fp)){
                                                break;
                                            }
                                            if(c!='"'){//keeping attribute name and value information
                                                if(c == 92){//some entries have \" in them which messes with value output
                                                    if(strlen(buff_val)==0){
                                                        strcpy(buff_val, (char*)&c);
                                                    }else{
                                                        strcat(buff_val, (char*)&c);
                                                    }
                                                    bufflimit--;
                                                    if(bufflimit == 0){
                                                        reallcount++;
                                                        bufflimit = BUFFERCAP-1;
                                                        buff_val = realloc(buff_val, reallcount*BUFFERCAP*sizeof(char));
                                                        if(buff_val == NULL){
                                                            printf("What???\n");
                                                        }
                                                    }
                                                    c = fgetc(fp);
                                                    if(c== '"'){
                                                        if(strlen(buff_val)==0){
                                                            strcpy(buff_val, (char*)&c);
                                                        }else{
                                                            strcat(buff_val, (char*)&c);
                                                        }
                                                        bufflimit--;
                                                        if(bufflimit == 0){
                                                            reallcount++;
                                                            bufflimit = BUFFERCAP-1;
                                                            buff_val = realloc(buff_val, reallcount*BUFFERCAP*sizeof(char));
                                                            if(buff_val == NULL){
                                                                printf("What???\n");
                                                            }
                                                        }
                                                        continue;
                                                    }
                                                    
                                                }
                                                if(strlen(buff_val)==0){
                                                    strcpy(buff_val, (char*)&c);
                                                }else{
                                                    strcat(buff_val, (char*)&c);
                                                }
                                                bufflimit--;
                                                if(bufflimit == 0){
                                                    reallcount++;
                                                    bufflimit = BUFFERCAP-1;
                                                    // printf("reallcount = %d\n", reallcount);
                                                    buff_val = realloc(buff_val, reallcount*BUFFERCAP*sizeof(char));
                                                    if(buff_val == NULL){
                                                        printf("What???\n");
                                                    }
                                                }
                                            }else{
                                                ecount--;
                                            }
                                        }
                                    }                                
                                }
                                //////////////////////
                            }while(1);
                            // node_tuple* temp = tulist->head;
                            // while(temp){
                                // printf("Attr: %s, val: %s.\n", temp->name, temp->value);
                                // temp = temp->next;
                            // }
                            // συνδεω το ταπλλιστ με το pseudonode (pseudonode->content = tuplelist)
                            // δλδ μπαινει "απευθειας" στο ^
                            
                            //printf("Out of loop\n");
                            //if(fclose(jfp)!= 0){
                               // fprintf(stderr, "Couldn't close JSON file.\n");
                           // }
                            ////////////////////////////////////////////////////////////////////////////
                            if(fclose(fp)!= 0){
                                fprintf(stderr, "Couldn't close file.\n");
                            }
                            list_destroy(&tulist, &error);
                            files_trav++;
                            printf("files_trav = %d\n", files_trav);
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

TEST_LIST = {
    {"csv_parse_first_line_test", test_firstlineparse},
    {"csv_eof_reaching", test_eofparse},
    {"csv_matching_lines_large",test_matchinglines_large},
    {"csv_matching_lines_medium",test_matchinglines_medium},
    {"JSON_Parser",test_JSONparse},
    {NULL, NULL}
};