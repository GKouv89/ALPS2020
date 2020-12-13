#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

typedef struct vector{
    int capacity;
    int size;
    int *elements;
} Vector;

void create_vector(Vector **);
void insert_element(Vector *);
void destroy_vector(Vector **);
#endif
