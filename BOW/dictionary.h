#ifndef DICTIONARY_H
#define DICTIONARY_H

typedef struct dict_cont{
    char *word;
    int payload;
} content;

typedef struct tnode{
    content node_cont;
    struct tnode *left_child;
    struct tnode *right_child;
} tree_node;

void create_tree(tree_node **);
int isEmpty_tree(tree_node *);
void destroy_tree(tree_node **);
void insert_tree(tree_node **, char *, int, int *);
int search_tree(tree_node *, char *);
#endif
