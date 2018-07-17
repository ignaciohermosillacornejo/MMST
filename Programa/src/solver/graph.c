#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Graph *graph_init(int C, int V, int E) // O (V + E)
{
    Graph *graph = malloc(sizeof(Graph));

    graph->c = C;
    graph->v = V;
    graph->e = E;
    graph->edges = (Edge *)malloc(graph->e * sizeof(Edge));
    graph->parent = (int *)malloc(graph->v * sizeof(int));
    graph->rank = (int *)malloc(graph->v * sizeof(int));

    for (int i = 0; i < graph->e; i++)
    {
        graph->edges[i].id = -1;
        graph->edges[i].s = -1;
        graph->edges[i].d = -1;
        graph->edges[i].c = -1;
    }
    for (int i = 0; i < graph->v; i++)
    {
        graph->parent[i] = i; // all elements are parents of it self initially
        graph->rank[i] = 0; // all elements have rank 0 at the begining
    }
    return graph;
}

void graph_insert(Graph *graph, int id, int v1, int v2, int c) // O(1)
{
    graph->edges[id].id = id;
    graph->edges[id].s = v1;
    graph->edges[id].d = v2;
    graph->edges[id].c = c;
}


int edge_comparator(const void *a, const void *b)
{
    Edge edge1 = *((Edge *)a);
    Edge edge2 = *((Edge *)b);
    return edge1.c - edge2.c;
}

void graph_sort(Graph *graph) // O (E log E)
{
    qsort((Edge *) graph->edges, graph -> e, sizeof(Edge), edge_comparator);
}

/* find the parent of a node using path compresion */
int find(Graph *graph, int v) // O (log V)
{
    if (graph->parent[v] != v)
    {
        graph->parent[v] = find(graph, graph->parent[v]);
    }
    return graph->parent[v];
}

/* merge two sets using rank
 * modified slightly so that any node that's a fountain 
 * becomes the parent of the group
 */
void merge(Graph *graph, int v1, int v2) // O (log V)
{
    int parent1 = find(graph, v1);
    int parent2 = find(graph, v2);

    /* we check here to make sure that if one of the parents is a fountain, it becomes the new head of the joined set */
    if (parent1 >= graph -> c) // this means that parent1 is a fountain node
    {
        graph->parent[parent2] = parent1;
        return;
    }
    else if (parent2 >= graph -> c) // this means that parent2 is a fountain node
    {
        graph->parent[parent1] = parent2;
        return;
    }

    /* normal union find from down here */
    if (graph->rank[parent1] < graph->rank[parent2]) // 1 is smaller than 2, atach 1 to the root of two
    {
        graph->parent[parent1] = parent2;
    }
    else
    {
        graph->parent[parent2] = parent1;
        graph->rank[parent1]++;
    }
}

void graph_kruskal(Graph *graph)
{
    graph_sort(graph); // sort our edges
    FILE *output_file = fopen("result.txt", "w");
    int mst_cost = 0;
    int count = 0; // count the number of edges in our MST

    // iterate over all edges
    for (int i = 0; i < graph -> e; i++)
    {
        int v1 = graph->edges[i].s;
        int v2 = graph->edges[i].d;

        int set1 = find(graph, v1);
        int set2 = find(graph, v2);

        /* We check for a cycle and that at least one of the 
        * sets doesn't have a a fountain as set leader
        */
        if (set1 != set2 &&  (set1 < graph->c || set2 < graph -> c))
        {
            mst_cost = mst_cost + graph->edges[i].c;
            fprintf (output_file, "%d\n", graph->edges[i].id);
            count = count + 1;
            if (count == graph -> c) // slight optimization, we break when we've reached the MST
            {
                printf("total cost: %d\n", mst_cost);
                //printf("total edges: %d\n", count);
                return;
            }
            merge(graph, set1, set2);
        }
    }
}

/* Free's all the memory used in the program */
void graph_destroy(Graph *graph)
{
    free(graph->edges);
    free(graph->parent);
    free(graph->rank);
    free(graph);
}

/* Debugg function */
void graph_print(Graph * graph)
{
    for (int i = 0; i < graph->e; i++)
    {
        Edge e = graph->edges[i];
        printf("(i: %d, v1: %d, v2: %d, c: %d)\n", e.id, e.s, e.d, e.c);
    }
}
