//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <dirent.h>
#include "../parse.h"
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

TEST_LIST = {
    {"csv_parse_first_line_test", test_firstlineparse},
    {"csv_eof_reaching", test_eofparse},
    {"csv_matching_lines_large",test_matchinglines_large},
    {"csv_matching_lines_medium",test_matchinglines_medium},
    {NULL, NULL}
};