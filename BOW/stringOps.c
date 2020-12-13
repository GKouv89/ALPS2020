#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stringOps.h"

void bow_it(char *buffer){
    char *word = calloc(255, sizeof(char));
    char c;
    int word_letters = 0;
    for(int i = 0; i < strlen(buffer); i++){
        c = *(buffer + i);
        if(isspace(c)){
            while(isspace(c)){
                i++;
                c = *(buffer + i);
            }
            i--;
            if(word_letters > 0){
                // printf("%s\n", word);
                memset(word, 0, 255*sizeof(char));
                word_letters = 0;
            }
        }else{
            if(c == '\\'){
                i++;
                c = *(buffer + i);
                if(c == 'n' || c == '"'){
                    if(word_letters > 0){
                        // printf("%s\n", word);
                        memset(word, 0, 255*sizeof(char));
                        word_letters = 0;    
                    }                    
                }
            }else if(isalpha(c)){
                c = tolower(c);
                memcpy(word + word_letters*sizeof(char), &c, sizeof(char));
                word_letters++;
            }
        }
    }
    if(word_letters > 0){ // a.k.a. there was no whitespace to indicate the need for printing
    // happened with buff_val
        // printf("%s\n", word);
    }
    free(word);
}