#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "queueelement.h"

int queue_empty(queue *q)
{
	assert(q); //making sure the queue exists
	return(q->front == NULL);
}

qnode* queue_create_node(qelem *item)
{
	qnode* qn = malloc(sizeof(qnode));
	qn->content = item;
	qn->next = NULL;
	qn->prev = NULL;
	return qn;
}

void queue_create(queue **q, int *error)
{
	if (*q) {
		*error = 1;
	} else {
		*error = 0;
		*q = malloc(sizeof(queue));
		(*q)->front = NULL;
		(*q)->rear = NULL;
	}
}

void queue_destroy_node(qnode **qn)
{
	free(*qn);
	*qn = NULL;
}

void queue_destroy(queue **q)
{
	qnode* temp;
	// while ((*q)->front) {
		// temp = (*q)->front;
		// (*q)->front = (*q)->front->next;
		// queue_destroy_node(&temp);
	// }
  // for our application, we will have to 
  // make sure no jobs are left in the queue
  assert(queue_empty(*q) == 1);
	free(*q);
	*q = NULL;
}

void queue_insert(queue *q, qelem *item) //inserts node at rear of queue
{
	assert(q);
	qnode* qn = queue_create_node(item);
	if (q->rear == NULL) { //if the list is empty
		q->front = qn;
		q->rear = qn;
	} else {
		q->rear->next = qn;
		qn->prev = q->rear;
		q->rear = qn;
	}
}
	

qelem* queue_remove(queue *q, int *error)
{
	qnode* qn;
	qelem *item;

	assert(q);
	if (queue_empty(q)) {
		*error = 1;
	} else {
		*error = 0;
		qn = q->front;
		q->front = q->front->next;
		if (q->front) {
			// There exists at least one more item in queue
			q->front->prev = NULL;
		} else {
			// Empty queue
			q->rear = NULL;
		}
		item = qn->content;
		queue_destroy_node(&qn);
	}
	return item;
}

int queue_size(queue *q)
{
	int size = 0;
	/* Insert code*/
	qnode *qn;

	assert(q);
	qn = q->front;
	while(qn){
		size+=1;
		qn = qn->next;
	}
	return size;
}
