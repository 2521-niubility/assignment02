// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static NodeValues newNode(int nV);

NodeValues outDegreeCentrality(Graph g) {
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues out = newNode(numVerticies(g));
    for (int i = 0; i < numVerticies(g); i++) {
        for (AdjList curr = outIncident(g,i); curr != NULL; curr = curr->next) {
            out.values[i] = out.values[i] + 1;
        }
    }
    return out;
}

NodeValues inDegreeCentrality(Graph g) {
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues in = newNode(numVerticies(g));
    for (int i = 0; i < numVerticies(g); i++) {
        for (AdjList curr = inIncident(g,i); curr != NULL; curr = curr->next) {
            in.values[i] = in.values[i] + 1;
        }
    }
    return in;
}

NodeValues degreeCentrality(Graph g) {
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues dc = newNode(numVerticies(g));
    for (int i = 0; i < numVerticies(g); i++) {
        dc.values[i] = outDegreeCentrality(g).values[i]+inDegreeCentrality(g).values[i];
    }
    return dc;
}

NodeValues closenessCentrality(Graph g) {
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues closeness = newNode(numVerticies(g));
    double noNodes = numVerticies(g);
    double reach = 0;
    double total = 0;
    for (int src = 0; src < noNodes; src++) {
        ShortestPaths p = dijkstra(g, src);
        for (int dest = 0; dest < noNodes; dest++) {
            if (p.dist[dest] != 0) {
                reach++;
                total = total + p.dist[dest];
            }
        }
        if (total == 0) {
            closeness.values[src] = 0;
        } else {
            closeness.values[src] = ((reach - 1)*(reach - 1))/((noNodes - 1)*total);
        }
    }
    return closeness;
}

NodeValues betweennessCentrality(Graph g){
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    int nV = numVerticies(g);
    int count1, count2;
    double value;
    NodeValues betweenness = newNode(numVerticies(g));
    for (int i = 0; i < nV; i++) {
        for (int j = 0;j < nV; j++) {
            ShortestPaths p = dijkstra(g, i);
            for (int k = 0; p.pred[k] != NULL; k++) {
                for (int l = 0; p.pred[k][l].next != NULL; l++) {
                    if (p.pred[k][l].v == i) {
                        count2++;
                    }
                    count1++;
                }
            }
        }
        value = count1/count2;
        betweenness.values[i] = value;
    }
    return betweenness;
}

NodeValues betweennessCentralityNormalised(Graph g){
     if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues normalised = newNode(numVerticies(g));
    double noNodes = numVerticies(g);
    NodeValues bcn = betweennessCentrality(g);
    for (int i = 0; i < noNodes; i++) {
        normalised.values[i] = bcn.values[i]/(noNodes - 1)/(noNodes - 2);
    }
    return normalised;
}

void showNodeValues(NodeValues values){
    for (int i = 0; i < values.noNodes; i++){
        printf("%d: %lf\n", i, values.values[i]);
    }
}

void freeNodeValues(NodeValues values){
    free(values.values);
}

// Set a new node value
static NodeValues newNode(int nV){
    NodeValues new;
    new.noNodes = nV;
    new.values = malloc(nV*sizeof(double));
    assert(new.values != NULL);
    for (int i = 0; i < nV; i++){
        new.values[i] = 0;
    }
    return new;
}
