#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../datatypes.h"
//#include "set.h"
#include "matchlist.h"


void matchlist_create(struct node **lnode){
    (*lnode)->matches = malloc(sizeof(matchlist));
    (*lnode)->matches->head = NULL;
    (*lnode)->matches->last = NULL;
}

match_tuple* create_match_tuple(struct node *matched_with, double prediction){
    match_tuple* new_match = malloc(sizeof(match_tuple));
    new_match->matched_with = matched_with;
    new_match->prediction = prediction;
    new_match->next = NULL;
    return new_match;
}

void matchlist_add(struct node *matcher, struct node *matched_with, double prediction){
    if(matcher->matches->head == NULL){
      matcher->matches->head = create_match_tuple(matched_with, prediction); 
      matcher->matches->last = matcher->matches->head;
      if(matched_with->matches->head == NULL){
          matched_with->matches->head = create_match_tuple(matcher, prediction);
          matched_with->matches->last = matched_with->matches->head;
      }else{
          matched_with->matches->last->next = create_match_tuple(matcher, prediction);
          matched_with->matches->last = matched_with->matches->last->next;
      }
    }else{
      matcher->matches->last->next = create_match_tuple(matched_with, prediction);
      matcher->matches->last = matcher->matches->last->next;
      if(matched_with->matches->head == NULL){
          matched_with->matches->head = create_match_tuple(matcher, prediction);
          matched_with->matches->last = matched_with->matches->head;
      }else{
          matched_with->matches->last->next = create_match_tuple(matcher, prediction);
          matched_with->matches->last = matched_with->matches->last->next;
      }
    }
}

void normalize(struct node *matcher, double max_pred){
  if(matcher->matches == NULL){
    return;
  }
  match_tuple *temp = matcher->matches->head;
  while(temp){
    temp->prediction = temp->prediction/max_pred;
    temp = temp->next;
  }
}

void delete_match_tuple(match_tuple **match){
    (*match)->matched_with = NULL;
    (*match)->next = NULL;
    free(*match);
    *match = NULL;
}

void destroy_matchlist(struct node **matcher){
    if((*matcher)->matches == NULL){
      return;
    }
    match_tuple* temp = (*matcher)->matches->head;
    if(temp != NULL && (*matcher)->matches->last != NULL){
        match_tuple* next = NULL;
        while(temp){
            next = temp->next;
            if(next){
              temp->next = NULL;
            }
            delete_match_tuple(&temp);
            temp = next;
        }
    }
    (*matcher)->matches->head = NULL;
    (*matcher)->matches->last = NULL;
    free((*matcher)->matches);
    (*matcher)->matches = NULL;
}

void matchlist_print(struct node *matcher){
    if(matcher->matches == NULL){
      return;
    }
    match_tuple *temp = matcher->matches->head;
    while(temp){
      printf("%s matched with %s with probability of %.16lf\n", matcher->id, temp->matched_with->id, temp->prediction);
      temp = temp->next;
    }
}


