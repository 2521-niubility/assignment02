// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

// The data type of counting number of times passing through v and shortest paths tracking
typedef struct Betweeness {
    double counterV; 
    double counterP; 
} Betweeness;

static NodeValues newNode(int nV);
static Betweeness countShortestPaths(ShortestPaths paths, Betweeness bw, int src, int dest, int v); 

// The number of ties that the node directs to others
NodeValues outDegreeCentrality(Graph g) {
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues out = newNode(numVerticies(g));
    int count;
    for (int i = 0; i < numVerticies(g); i++) {
        count = 0;
        for (AdjList curr = outIncident(g,i); curr != NULL; curr = curr->next) {
            count++;
            out.values[i] = count;
        }
    }
    return out;
}

// A count of the number of ties directed to the node
NodeValues inDegreeCentrality(Graph g) {
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues in = newNode(numVerticies(g));
    int count;
    for (int i = 0; i < numVerticies(g); i++) {
        count = 0;
        for (AdjList curr = inIncident(g,i); curr != NULL; curr = curr->next) {
            count++;
            in.values[i] = count;
        }
    }
    return in;
}

// The total number of indegree centrality and outdegree centrality
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

// Calculate the sum of the length of the shortest paths between the node and all other nodes
NodeValues closenessCentrality(Graph g) {
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues closeness = newNode(numVerticies(g));   
    double reach = 0;
    double total = 0;
    for (int src = 0; src < numVerticies(g); src++) {
        ShortestPaths p = dijkstra(g, src);
        reach = 0;
        total = 0;
        for (int dest = 0; dest < numVerticies(g); dest++) {
            if (src != dest) {
                if (p.dist[dest] > 0) {
                    reach++;
                    total = total + p.dist[dest];
                } 
            } else {
                reach++;
            }
        }        
        if (total == 0) {
            closeness.values[src] = 0;
        } else {
            closeness.values[src] = ((reach - 1)/(numVerticies(g) - 1))*((reach - 1)/total);
        }
    }
    return closeness;
}

// The total number of shortest paths between two different nodes.
NodeValues betweennessCentrality(Graph g) {
	if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues new = newNode(numVerticies(g));
    Betweeness bw;
    for (int v = 0; v < numVerticies(g); v++) {
        for (int src = 0; src < numVerticies(g); src++) {
            if (src != v) {
                ShortestPaths paths = dijkstra(g, src);
                for (int dest = 0; dest < numVerticies(g); dest++) {
                    if (src != dest && dest != v) {
                        bw.counterV = bw.counterP = 0;
                        bw = countShortestPaths(paths, bw, src, dest, v);
                        if (bw.counterP != 0) {
                            new.values[v] = new.values[v] + bw.counterV/bw.counterP;
                        }
                    }
                }
            }
        }
    }
    return new;
}

// Calculate normalised betweenness centrality
NodeValues betweennessCentralityNormalised(Graph g){
    if (g == NULL) {
        NodeValues throwAway = {0};
        return throwAway;
    }
    NodeValues normalised = betweennessCentrality(g);
    int i = 0;
    for (i = 0; i < normalised.noNodes; i++) {
        normalised.values[i] = normalised.values[i]/(numVerticies(g)-1)/(numVerticies(g)-2);
   }
   return normalised;
}

// Print node values list
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
    for (int i = 0; i < nV; i++){
        new.values[i] = 0;
    }
    return new;
}

// The inplementation of counting number of times passing through v and shortest paths tracking and return betweeness data struct
static Betweeness countShortestPaths(ShortestPaths paths, Betweeness bw, int src, int dest, int v) {
    PredNode *prev;  
    for (prev = paths.pred[dest]; prev != NULL; prev = prev->next) {
        if (prev->v == src) {
            bw.counterP++;
        } else if (prev->v == v && prev->v !=src) {
           bw.counterV++;
        }
        bw = countShortestPaths(paths, bw, src, prev->v, v); 
    }
    return bw;
}
