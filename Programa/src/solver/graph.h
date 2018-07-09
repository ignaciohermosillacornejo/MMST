#pragma once

typedef struct graph Graph;
typedef struct edge Edge;

struct edge
{
    int id; // for output
    int s;  // source
    int d;  // destination
    int c;  // cost
};

struct graph
{
    int v;      // number of vertices
    int c;      // number cities
    int e;      // number of edges
    Edge *edges; // array of size e to hold our edges
    int *parent; // array of size v to hold our parent array
    int *rank;  // array of size v to hold our rank array
};


Graph *graph_init(int C, int V, int E);
void graph_insert(Graph *graph, int id, int s, int d, int c);
void graph_kruskal(Graph *graph);
void graph_destroy(Graph *graph);
void graph_print(Graph * graph);