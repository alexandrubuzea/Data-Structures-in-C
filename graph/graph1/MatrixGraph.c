#include <stdio.h>
#include <stdlib.h>

#include "MatrixGraph.h"

// am lucrat tot in ipoteza grafului neorientat

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru matricea de adiacenta a grafului.
 */
matrix_graph_t*
mg_create(int nodes)
{
	matrix_graph_t *graph = (matrix_graph_t *)malloc(sizeof(matrix_graph_t));
	DIE(!graph, "Malloc failed\n");

	graph->matrix = (int **)malloc(nodes * sizeof(int *));
	DIE(!graph->matrix, "Malloc failed\n");

	for (int i = 0; i < nodes; ++i) {
		int *tmp = (int *)calloc(nodes, sizeof(int));
		DIE(!tmp, "Calloc failed\n");

		graph->matrix[i] = tmp;
	}

	graph->nodes = nodes;

	return graph;
}

/* Adauga o muchie intre nodurile sursa si destinatie */
void
mg_add_edge(matrix_graph_t* graph, int src, int dest)
{
	if (src == dest)
		return;

	if (!graph)
		return;

	graph->matrix[dest][src] = 1;
	graph->matrix[src][dest] = 1;
}

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int
mg_has_edge(matrix_graph_t* graph, int src, int dest)
{
	return graph->matrix[src][dest];
}

/* Elimina muchia dintre nodurile sursa si destinatie */
void
mg_remove_edge(matrix_graph_t* graph, int src, int dest)
{
	if (!graph)
		return;

	graph->matrix[src][dest] = 0;
	graph->matrix[dest][src] = 0;
}

/* Elibereaza memoria folosita de matricea de adiacenta a grafului */
void
mg_free(matrix_graph_t* graph)
{
	if (!graph)
		return;
	
	for (int i = 0; i < graph->nodes; ++i)
		free(graph->matrix[i]);
	
	free(graph->matrix);
	free(graph);
}