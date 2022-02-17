#include <stdlib.h>
#include <string.h>

#include "Queue.h"
#include "utils.h"

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *q = (queue_t *)malloc(sizeof(queue_t));
	DIE(!q, "Malloc failed\n");

	q->max_size = max_size;
	q->size = 0;
	q->data_size = data_size;
	q->read_idx = 0;
	q->write_idx = 0;

	q->buff = (void **)calloc(max_size, sizeof(void *));
	DIE(!q->buff, "Malloc failed\n");

	return q;
}

unsigned int
q_get_size(queue_t *q)
{
	return q->size;
}

/*
 * Intoarce 1 daca coada este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q)
{
	if (q->size == 0)
		return 1;
	return 0;
}

void *
q_front(queue_t *q)
{
	if (q_is_empty(q) == 1)
		return NULL;
	
	return q->buff[q->read_idx];
}

/* Functia intoarce true daca operatia s-a efectuat si false in caz contrar */
bool
q_dequeue(queue_t *q)
{
	if (q->size == 0)
		return false;
	
	free(q->buff[q->read_idx]);
	q->buff[q->read_idx] = NULL;
	q->read_idx++;

	if (q->read_idx == q->max_size)
		q->read_idx = 0;
	
	q->size--;
	return true;
}

/* Functia intoarce true daca operatia s-a efectuat si false in caz contrar */
bool
q_enqueue(queue_t *q, void *new_data)
{
	if (q->size == q->max_size)
		return false;

	int index;
	if (q->write_idx == q->max_size)
		index = 0;
	else
		index = q->write_idx;

	void *tmp = malloc(q->data_size * sizeof(char));
	DIE(!tmp, "Malloc failed\n");

	q->buff[index] = tmp;
	memcpy(q->buff[index], new_data, q->data_size);

	q->write_idx = index + 1;
	q->size++;
	return true;
}

void
q_clear(queue_t *q)
{
	while (!q_is_empty(q))
		q_dequeue(q);
}

void
q_free(queue_t *q)
{
	q_clear(q);
	free(q->buff);
	free(q);
}
