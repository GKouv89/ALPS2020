#ifndef __VECTOROPS_H__
#define __VECTOROPS_H__


#ifndef CAPACITY
    #define CAPACITY 100
#endif

typedef struct vector{
    int capacity;
    int size;
    int *elements;
} Vector;

void create_vector(Vector **);
void new_word(Vector *, int *);
void update_element(Vector *, int, int *);
void resize_vector(Vector *);
void destroy_vector(Vector **);
#endif
