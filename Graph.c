// Graph ADT interface for Ass2 (COMP2521)
// Some source may used from COMP2521 19s1 Lab07 & Lab08.
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <sysexits.h>

#include "Graph.h"
static bool validV(Graph g, Vertex v);
struct GraphRep{
    int     nV;     /* number of vertexs */
    int   **edges;  /* matrix */
};


Graph newGraph(int noNodes) {
    /* allocate for graph */
    Graph new = malloc (sizeof(*new));
    if (new == NULL) err(EX_OSERR, "Couldn't allocate graph.");
    new->nV = noNodes;

    /* allocate for EDGEs */
    new->edges = calloc(noNodes, sizeof(int *));
    if (new->edges == NULL) err(EX_OSERR, "calloc error for edges");

    /* allocate for each edge and initialise to 0 */
    for (int i = 0; i < noNodes; i++) {
        new->edges[i] = calloc (noNodes, sizeof(int *));
        if (new->edges[i] == NULL) err(EX_OSERR, "calloc error for edge");
        for (int j = 0; j < noNodes; j++)
            new->edges[i][j] = 0;
    }
    return new;
}

int numVerticies(Graph g) {
    assert(g != NULL);
    return g->nV;
}

void insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
    assert((g != NULL) && validV(g, src) && validV(g, dest));
    if (g->edges[src][dest] == 0) g->edges[src][dest] = weight;

}

void removeEdge(Graph g, Vertex src, Vertex dest) {
    assert((g != NULL) && validV(g, src) && validV(g, dest));
    if (g->edges[src][dest] != 0) g->edges[src][dest] = 0;
}

bool adjacent(Graph g, Vertex src, Vertex dest) {
    assert(g != NULL);
    return (g->edges[src][dest] != 0);
}

AdjList outIncident(Graph g, Vertex v) {
    assert(g != NULL && validV(g, v));
    adjListNode *now = NULL;
    adjListNode *first = NULL;
    for (Vertex i = 0; i < g->nV; i++) {
        if (adjacent(g, v, i)) {
            if (first != NULL) {
                now->next = malloc(sizeof(adjListNode));
                now = now->next;
            }
            else
                now = malloc(sizeof(adjListNode));
            now->w = i;
            now->weight = g->edges[v][i];
            now->next = NULL;
            if (first == NULL) first = now;
        }
    }
    return first;
}

AdjList inIncident(Graph g, Vertex v) {
    assert(g != NULL && validV(g, v));
    adjListNode *now = NULL;
    adjListNode *first = NULL;
    for (Vertex i = 0; i < g->nV; i++) {
        if (adjacent(g, i, v)) {
            if (first != NULL) {
                now->next = malloc(sizeof(adjListNode));
                now = now->next;
            }
            else
                now = malloc(sizeof(adjListNode));
            now->w = i;
            now->weight = g->edges[i][v];
            now->next = NULL;
            if (first == NULL) first = now;
        }
    }
    return first;
}
void showGraph(Graph g) {
    assert (g != NULL);
    if (g->nV == 0) {
        puts ("Graph is empty");
        return;
    }

    /* if not empty */
    printf("\t");
    for (int i = 0; i < g->nV; i++) {
        printf("\033[1;32m%d\t\033[0m", i);
    }
    printf("\n");
    for (int i = 0; i < g->nV; i++) {
        for (int j = 0; j < g->nV; j++) {
            if (j == 0) printf("\033[1;32m%d\t\033[0m", i);
            if (j == i) printf("\033[31mX\t\033[0m");
            else printf ("%-3d\t", g->edges[i][j]);
        }
        printf("\n");
    }
}

void freeGraph(Graph g) {
    assert(g != NULL);
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }

    free(g->edges);
    free(g);
}

static bool validV(Graph g, Vertex v) {
    return (g != NULL && v >= 0 && v < g->nV);
}
