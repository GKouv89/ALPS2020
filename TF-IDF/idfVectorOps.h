#ifndef __IDFVECTOROPS_H__
#define __IDFVECTOROPS_H__

#ifndef CAPACITY
    #define CAPACITY 100
#endif

#ifndef FILES
    #define FILES 29787
#endif

typedef struct idf_vector{
    char *name;
    int capacity;
    int size;
    double *elements;
} IDFVector;

void create_idf_vector(IDFVector **);
IDFVector* crop_idf_vector(IDFVector *, int);
IDFVector* concatenate_idf_vectors(IDFVector *, IDFVector *);
void update_idf_vector(IDFVector *, int);
void resize_idf_vector(IDFVector *);
void compute_idf_vals(IDFVector *);
void destroy_idf_vector(IDFVector **);
#endif
