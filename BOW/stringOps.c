#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stringOps.h"

void bow_it(char *buffer, sw_list *l, tree_node **tree, BoW **bag, int text_num){
    char *word = calloc(255, sizeof(char));
    char c;
    int word_letters = 0;
    static int word_counter = 0;
    int error = 0;
    int payload;
    for(int i = 0; i < strlen(buffer); i++){
        c = *(buffer + i);
        if(isspace(c)){
            while(isspace(c)){
                i++;
                c = *(buffer + i);
            }
            i--;
            if(word_letters > 0){
                if(!is_stopword(l, word)){
                    // printf("%s\n", word);
                    insert_tree(tree, word, word_counter, &error);
                    if(error == 1){
                        // Duplicate, word already in bag
                        payload = search_tree((*tree), word);
                        old_word((*bag), text_num, payload);
                    }else{
                        word_counter++;                        
                        new_word_in_bag((*bag), text_num);
                    }
                }
                memset(word, 0, 255*sizeof(char));
                word_letters = 0;
            }
        }else{
            if(c == '\\'){
                i++;
                c = *(buffer + i);
                if(c == 'n' || c == '"'){
                    if(word_letters > 0){
                        if(!is_stopword(l, word)){
                            insert_tree(tree, word, word_counter, &error);
                            if(error == 1){
                                // Duplicate, word already in bag
                                payload = search_tree((*tree), word);
                                old_word((*bag), text_num, payload);
                            }else{
                                word_counter++;                        
                                new_word_in_bag((*bag), text_num);
                            }    
                        }
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
        insert_tree(tree, word, word_counter, &error);
        if(error == 1){
            // Duplicate, word already in bag
            payload = search_tree((*tree), word);
            old_word((*bag), text_num, payload);
        }else{
            word_counter++;                        
            new_word_in_bag((*bag), text_num);
        }
    }
    free(word);
    // printf("unique word_counter :%d\n", word_counter);
}