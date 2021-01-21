#ifndef QUEUE_H_
#define QUEUE_H_

#include "queueelement.h"

typedef struct qnode {
	qelem content;
	struct qnode *next;
	struct qnode *prev;
} qnode;

typedef struct queue {
	qnode *front;
	qnode *rear;
} queue;

int queue_empty(queue *);
qnode* queue_create_node(qelem);
void queue_create(queue **, int *);
void queue_insert(queue *, qelem);
void queue_destroy_node(qnode **);
void queue_destroy(queue **);
qelem queue_remove(queue *, int *);
int queue_size(queue *);

#endif
