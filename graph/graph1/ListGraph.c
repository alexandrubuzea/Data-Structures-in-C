#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"

// am presupus graful ca fiind neorientat

list_graph_t*
lg_create(int nodes)
{
	list_graph_t *graph = (list_graph_t *)malloc(sizeof(list_graph_t));
	DIE(!graph, "Malloc failed\n");

	graph->nodes = nodes;

	linked_list_t **tmp = (linked_list_t **)
						 malloc(nodes * sizeof(linked_list_t *));
	DIE(!tmp, "Malloc failed\n");

	graph->neighbors = tmp;

	for (int i = 0; i < nodes; ++i)
		graph->neighbors[i] = ll_create(sizeof(int));

	return graph;
}

void
lg_add_edge(list_graph_t* graph, int src, int dest)
{
	if (src == dest)
		return;

	ll_node_t *it = graph->neighbors[src]->head;
	int cnt = 0;

	while (it != NULL) {
		int *index = (int *)it->data;
		if (*index > dest) {
			ll_add_nth_node(graph->neighbors[src], cnt, &dest);
			break;
		}

		if (*index == dest)
			return; // exista deja muchia dorita, nu este nevoie sa continuam

		++cnt;
		it = it->next;
	}

	if (it == NULL) {
		ll_add_nth_node(graph->neighbors[src], cnt, &dest);
	}

	it = graph->neighbors[dest]->head;
	cnt = 0;

	while (it != NULL) {
		int *index = (int *)it->data;
		if (*index > src) {
			ll_add_nth_node(graph->neighbors[dest], cnt, &src);
			break;
		}

		++cnt;
		it = it->next;
	}

	if (it == NULL) {
		ll_add_nth_node(graph->neighbors[dest], cnt, &src);
	}
}

int
lg_has_edge(list_graph_t* graph, int src, int dest)
{
	ll_node_t *it = graph->neighbors[src]->head;
	while (it != NULL) {
		int *index = (int *)it->data;
		if (*index > dest)
			return 0;

		if (*index == dest)
			return 1;

		it = it->next;
	}

	return 0;
}

linked_list_t*
lg_get_neighbours(list_graph_t* graph, int node)
{
	if (!graph)
		return NULL;

	return graph->neighbors[node];
}

void
lg_remove_edge(list_graph_t* graph, int src, int dest)
{
	ll_node_t *it = graph->neighbors[src]->head;
	int cnt = 0;
	while (it != NULL) {
		int *index = (int *)it->data;
		if (*index == dest) { // daca am gasit latura
			it = ll_remove_nth_node(graph->neighbors[src], cnt);
			free(it->data);
			free(it);
			break;
		}
		if (it->next == NULL || *index < dest)  // nu avem aceasta latura
			return;

		++cnt;
		it = it->next;
	}

	it = graph->neighbors[dest]->head;
	cnt = 0;

	while (it != NULL) {
		int *index = (int *)it->data;

		if (*index == src) {
			it = ll_remove_nth_node(graph->neighbors[dest], cnt);
			free(it->data);
			free(it);
			break;
		}
		++cnt;
		it = it->next;
	}
}

void
lg_free(list_graph_t* graph)
{
	if (!graph)
		return;
	
	if (!graph->neighbors) {
		free(graph);
		return;
	}

	for (int i = 0; i < graph->nodes; ++i)
		ll_free(graph->neighbors + i);
	
	free(graph->neighbors);
	free(graph);
}