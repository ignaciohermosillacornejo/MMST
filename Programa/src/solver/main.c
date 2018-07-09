#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Modo de uso: ./solver test.txt\n");
		return 0;
	}

	FILE *input_file = fopen(argv[1], "r");

	if (!input_file)
	{
		printf("¡El archivo %s no existe!\n", argv[1]);
		return 2;
	}

	int c; int f; int e;
	fscanf(input_file, "%d %d %d", &c, &f, &e);

	Graph * graph = graph_init(c, c + f, e);

	for (int i = 0; i < e; i++)
	{
		int v1; int v2; int cost;
		fscanf(input_file, "%d %d %d", &v1, &v2, &cost);
		graph_insert(graph, i, v1, v2, cost); 
	}

	graph_kruskal(graph);
	graph_destroy(graph);

	// return ok
	return 0;
}
