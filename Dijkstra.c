// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define MAXDISTANCE 1<<16

ShortestPaths dijkstra(Graph g, Vertex v) {
    int N = numVerticies(g);

    /* MEMORY ALLOCATE */
    ShortestPaths *way = malloc(sizeof(ShortestPaths));
    way->noNodes = N;
    way->src = v;
    way->dist = malloc(sizeof(int) * N);
    way->pred = calloc(N, sizeof(PredNode));

    /* INITIALISE FOR vSet */
    // stores the vertexes that have not been seen yet
    PQ vSet = newPQ();
    for (int i = 0; i < N; i++) {
        ItemPQ newitem = {.key = i, .value = (i == v) ? 0 : MAXDISTANCE};
        addPQ(vSet, newitem);
        way->dist[i] = (i == v) ? 0 : MAXDISTANCE;
        way->pred[i] = malloc(sizeof(PredNode));
        way->pred[i]->v = -1;
        way->pred[i]->next = NULL;
    }

    /* MAIN PROCEDURE */
    while (!PQEmpty(vSet)) {
        ItemPQ it = dequeuePQ(vSet);
        for (AdjList curr = outIncident(g, it.key); curr != NULL; curr = curr->next) {
            PredNode *c = malloc(sizeof(PredNode));

            // if the new distance is less than current value,
            // the (all) current node(s) should be freed and updated to new node
            // and it will always be the first node of that link
            if (it.value + curr->weight < way->dist[curr->w]) {
                PredNode *i = way->pred[curr->w];
                while (i != NULL) {
                    PredNode *k = i;
                    i = i->next;
                    free(k);
                }
                way->dist[curr->w] = it.value + curr->weight;
                ItemPQ update = {.key = curr->w, .value = way->dist[curr->w]};
                updatePQ(vSet, update);
                c->v = it.key; c->next = NULL;
                way->pred[curr->w] = c;
            }

            // if the new distance is the same as the current value,
            // connect it to the last node at current index
            else if (it.value + curr->weight == way->dist[curr->w]) {
                PredNode *t = way->pred[curr->w];
                for (; t->next != NULL; t = t->next);
                t->next = c;
                c->v = it.key; c->next = NULL;
            }
            else
                continue;
        }
    }

    // for those vertexes that are not able to reach from start point
    // set the distance to 0 and pred to NULL
    for (int i = 0; i < N; i++) {
        if (way->pred[i]->v == -1) {
            way->dist[i] = 0;
            way->pred[i] = NULL;
        }
    }

    freePQ(vSet);
    return *way;
}

void showShortestPaths(ShortestPaths paths) {
    printf("\033[0;33mNode %d\033[0m\n", paths.src);
    printf("  \033[1;32mDistance\033[0m\n");
    for (int i = 0; i < paths.noNodes; i++) {
        printf("    %d : ", i);
        if (i == paths.src)
            printf("X\n");
        else
            printf("%d\n", paths.dist[i]);
    }

    printf("  \033[1;32mPreds\033[0m\n");
    for (int i = 0; i < paths.noNodes; i++) {
        printf("    %d : ", i);
        struct PredNode *curr = paths.pred[i];
        for (; curr != NULL; curr = curr->next) {
            printf("[%d]->", curr->v);
        }
        if (curr == NULL)  {
            printf("NULL\n");
            continue;
        }
        // printf("NULL\n");
    }
}


void freeShortestPaths(ShortestPaths paths) {
    free(paths.dist);
    for (int i = 0; i < paths.noNodes; i++) {
        free(paths.pred[i]);
    }
    free(paths.pred);
}
