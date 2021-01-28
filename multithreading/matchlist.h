#ifndef MATCHLIST_H
#define MATCHLIST_H

typedef struct match{
    struct node* matched_with;
    double prediction;
    struct match *next;
} match_tuple;

typedef struct amatchlist{
    match_tuple *head;
    match_tuple *last;
}matchlist;

void matchlist_create(struct node**);
match_tuple* create_match_tuple(struct node *, double);
void matchlist_add(struct node*, struct node*, double);
void normalize(struct node *, double);
void neglist_print(struct node*);
void delete_match_tuple(match_tuple**);
void destroy_matchlist(struct node**);
#endif