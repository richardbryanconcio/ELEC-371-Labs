#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void PrintGraph(Graph *g) {
    int i;
    Edge *p;

    printf ("graph has %d vertices and %d edges\n",
        g->num_vertices, g->num_edges);
    printf ("list of edges:\n");
    for (i = 0; i < g->num_vertices; i++)
    {
        for (p = g->v[i].adj; p != NULL; p = p->next)
            if (p->other_vertex < p->vertex)
                continue;
            else
                printf(" (%d, %d)\n", p->vertex, p->other_vertex);
    }
}

void PrepareEdgeList(Graph *g, Edge **list) {
    int i;
    int edgeCount = 0;
    Edge *p;

    printf ("graph has %d vertices and %d edges\n", g->num_vertices, g->num_edges);
    printf ("list of edges:\n");

    for (i = 0; i < g->num_vertices; i++) {
        for (p = g->v[i].adj; p != NULL; p = p->next) {
            if (p->other_vertex < i) {
                continue; // Skip duplicate edges
            } else {
                list[edgeCount++] = p; // Add the edge to the list
            }
        }
    }
}