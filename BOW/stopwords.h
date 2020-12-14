#ifndef STOPWORDS_H
#define STOPWORDS_H

typedef struct stopword_node{
    char *word;
    struct stopword_node *next;
} sw_node;

typedef struct stopword_list{
    sw_node *front;
    sw_node *back;
} sw_list;

void make_stopword_list(sw_list **);
void create(sw_list **);
void insert(sw_list *, char *);
int is_stopword(sw_list *, char *);
void print_sw(sw_list *);
void destroy(sw_list **);

#endif
