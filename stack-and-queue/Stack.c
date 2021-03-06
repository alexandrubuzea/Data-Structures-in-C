#include <stdlib.h>
#include <string.h>

#include "Stack.h"
#include "utils.h"

stack_t *
st_create(unsigned int data_size)
{
	stack_t *st = (stack_t *)malloc(sizeof(stack_t));
	DIE(!st, "Malloc failed\n");

	st->list = ll_create(data_size);
	return st;
}

unsigned int
st_get_size(stack_t *st)
{
	if (!st)
		return 0;
	
	return ll_get_size(st->list);
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int
st_is_empty(stack_t *st)
{
	if (st_get_size(st) != 0)
		return 0;
	return 1;
}

void *
st_peek(stack_t *st)
{
	void *ret_data = st->list->head->data;
	return ret_data;
}

void
st_pop(stack_t *st)
{	
	ll_node_t *rem = ll_remove_nth_node(st->list, 0);
	free(rem->data);
	free(rem);
}

void
st_push(stack_t *st, void *new_data)
{
	ll_add_nth_node(st->list, 0, new_data);
}

void
st_clear(stack_t *st)
{	
	while (!st_is_empty(st))
		st_pop(st);
}

void
st_free(stack_t *st)
{
	st_clear(st);
	free(st->list);
	free(st);
}
